/*
 * 	fl_distortion_surface.cpp
 *
 */

#include <SDL2/SDL.h>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "fl_particle_surface.h"

#include "../logging/logging.h"
#include "fl_particle_shader.h"
#include "renderer.h"
#include "texture.h"
#include "textured_object.h"

#define VERT_SIZE 3	   // x, y, life
#define RESTART 0xFFFF // opengl primitive restart index

FLParticleSurface::FLParticleSurface(unsigned int num_particles,
									 unsigned int particle_life,
									 float particle_size) {
	this->num_particles = num_particles;
	this->particle_life = particle_life;
	this->particle_size = particle_size;
	init_particle_field();
	shader = nullptr;
	next_loc = 0;
}

void FLParticleSurface::add_particle(float x, float y) {
	add_particle(x, y, 0, 0);
}

void FLParticleSurface::add_particle(float x, float y, float vx, float vy) {

	if (next_loc > num_particles)
		log_error("location out of range of particle surface");

	particle_field[next_loc].xs.clear();
	particle_field[next_loc].ys.clear();

	// We default to creating a quad with 4 vertices
	// v1
	particle_field[next_loc].xs.push_back(x);
	particle_field[next_loc].ys.push_back(y);

	// v2
	particle_field[next_loc].xs.push_back(x + particle_size);
	particle_field[next_loc].ys.push_back(y);

	// v3
	particle_field[next_loc].xs.push_back(x + particle_size);
	particle_field[next_loc].ys.push_back(y + particle_size);

	// v4
	particle_field[next_loc].xs.push_back(x);
	particle_field[next_loc].ys.push_back(y + particle_size);

	particle_field[next_loc].life = particle_life;
	particle_field[next_loc].vx = vx;
	particle_field[next_loc].vy = vy;

	if (++next_loc >= num_particles)
		next_loc = 0;
}

void FLParticleSurface::init_particle_field() {
	srand(time(NULL));

	particle_field = std::vector<fl_particle>(num_particles,
											  fl_particle{
												  std::vector<float>(), // xs
												  std::vector<float>(), // ys
												  0,					// vx
												  0,					// vy
												  0						// life
											  });

	alt_particle_field = particle_field;
}

void FLParticleSurface::update_buffers() {
	if (shader == nullptr)
		log_warning("Updating particle surface with null shader\n");

	unsigned int index = 0;
	std::vector<float> vbuf;
	std::vector<unsigned int> ibuf;
	num_indices = 0;

	for (fl_particle p : particle_field) {
		if (p.life > 0) {
			for (unsigned int vert = 0; vert < p.xs.size(); ++vert) {
				vbuf.push_back(p.xs[vert]);
				vbuf.push_back(p.ys[vert]);
				vbuf.push_back(p.life);
				ibuf.push_back(index++);
				++num_indices;
			}

			ibuf.push_back(RESTART);
			++num_indices;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vbuf.size() * sizeof(float), &(vbuf[0]),
				 GL_STATIC_DRAW);

	if (glGetError() != GL_NO_ERROR)
		log_error("Error buffering vbo for particle surface");

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibuf.size() * sizeof(unsigned int),
				 &(ibuf[0]), GL_STATIC_DRAW);

	if (glGetError() != GL_NO_ERROR)
		log_error("Error buffering ibo");

	glBindVertexArray(vao);

	if (glGetError() != GL_NO_ERROR)
		log_error("Error binding VAO");

	glPrimitiveRestartIndex(RESTART);

	shader->enable_vertex_pointer();
	shader->enable_life_pointer();

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	shader->set_vertex_pointer(VERT_SIZE * sizeof(float), NULL);
	((FLParticleShader *)shader)
		->set_life_pointer(VERT_SIZE * sizeof(float),
						   (const void *)(2 * sizeof(float)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBindVertexArray(0);
}

void FLParticleSurface::render() {
	shader->bind();
	update_particle_field();
	update_buffers();
	if (shader != nullptr)
		shader->render(vao, num_indices);
	else
		log_warning("Attempted to render with null shader.");
}
