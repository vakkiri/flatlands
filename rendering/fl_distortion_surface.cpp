/*
 * 	fl_distortion_surface.cpp
 *
 */

#include <SDL2/SDL.h>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>

#include "fl_distortion_surface.h"

#include "../logging/logging.h"
#include "fl_smoke_background_shader.h"
#include "particles/particle.h"
#include "renderer.h"
#include "texture.h"
#include "textured_object.h"

#define NUM_PARTICLES (512 * 10)
#define PARTICLE_LIFE 600
#define VERT_SIZE 5

#define RESTART 0xFFFF

FLDistortionSurface::FLDistortionSurface() : FLTexturedSurface() {
	num_verts = NUM_PARTICLES * 4;		// square particles -> 4 verts
	num_indices = NUM_PARTICLES * 5; 	// 4 + RESTART
	init_particle_field();
}

FLDistortionSurface::~FLDistortionSurface() {
	clear_particle_field();
}

void FLDistortionSurface::clear_particle_field() {
	for (int i = 0; i < particle_field.size(); ++i) {
		if (particle_field[i] != nullptr)
			delete particle_field[i];
		if (alt_particle_field[i] != nullptr)
			delete particle_field[i];
	}

	particle_field.clear();
	alt_particle_field.clear();
	next_loc = 0;
}

void FLDistortionSurface::add_particle( float x, float y ) {
	add_particle(	x, 
			y, 
			(((float)(rand() % 77)) / 77.f) * 1.f, 
			(((float)(rand() % 77)) / 77.f) * 1.f);
}

void FLDistortionSurface::add_particle( float x, float y, float vx, float vy ) {
	unsigned int verts = 3;

	particle_field[next_loc]->xs.clear();
	particle_field[next_loc]->ys.clear();

	// v1
	particle_field[next_loc]->xs.push_back(x);
	particle_field[next_loc]->ys.push_back(y);

	// v2
	particle_field[next_loc]->xs.push_back(x + 6.f + (rand() % 8));
	particle_field[next_loc]->ys.push_back(y + (rand() % 8) - 4.f);

	// v3
	particle_field[next_loc]->xs.push_back(x + (rand() % 8));
	particle_field[next_loc]->ys.push_back(y + 6.f + (rand() % 8));

	particle_field[next_loc]->life = PARTICLE_LIFE;
	particle_field[next_loc]->vx = vx;
	particle_field[next_loc]->vy = vy;

	if ( ++next_loc >= NUM_PARTICLES )
		next_loc = 0;
}

void FLDistortionSurface::init_particle_field() {
	srand( time(NULL) );

	clear_particle_field();

	for ( int i = 0; i < NUM_PARTICLES; ++i ) {
		particle_field.push_back( new fl_particle{ std::vector<float>(), std::vector<float>(), 0.f, 0.f, 0 } );
		alt_particle_field.push_back( new fl_particle{ std::vector<float>(), std::vector<float>(), 0.f, 0.f, 0 } );
	}

}

void FLDistortionSurface::update_particle_field() {

	for ( int i = 0; i < NUM_PARTICLES; ++i ) { 
		alt_particle_field[i]->xs = particle_field[i]->xs;
		alt_particle_field[i]->ys = particle_field[i]->ys;
		alt_particle_field[i]->vx = particle_field[i]->vx;
		alt_particle_field[i]->vy = particle_field[i]->vy;
		alt_particle_field[i]->life = particle_field[i]->life;
		
		if ( alt_particle_field[i]->life > 0) {
			alt_particle_field[i]->life -= 1;

			for ( int j = 1; j < alt_particle_field[i]->xs.size(); ++j ) {
				alt_particle_field[i]->xs[j] += alt_particle_field[i]->vx;
				alt_particle_field[i]->ys[j] += alt_particle_field[i]->vy;
			}

			if (alt_particle_field[i]->ys[1] < alt_particle_field[i]->ys[2]) {
				alt_particle_field[i]->ys[1] -= 0.2;
				alt_particle_field[i]->ys[2] += 0.2;
			}
			else { 
				alt_particle_field[i]->ys[1] += 0.2;
				alt_particle_field[i]->ys[2] -= 0.2;
			}
			if (alt_particle_field[i]->xs[1] < alt_particle_field[i]->xs[2]) {
				alt_particle_field[i]->xs[1] -= 0.2;
				alt_particle_field[i]->xs[2] += 0.2;
			}
			else { 
				alt_particle_field[i]->xs[1] += 0.2;
				alt_particle_field[i]->xs[2] -= 0.2;
			}
		}

	}
	
	// set new updated field as current field
	std::swap( alt_particle_field, particle_field );

	// add 1 particle per frame
	float x = 768/2;
	float y = 512/2;
	float vx = (((float)(rand() % 78)) / 77.f) * -8.f + 4.f;
	float vy = (((float)(rand() % 78)) / 77.f) * -8.f + 4.f;
	add_particle(x, y, vx, vy);

}

void FLDistortionSurface::update_buffers() {

	unsigned int index = 0;
	std::vector<float> vbuf;
	std::vector<unsigned int> ibuf;

	for ( fl_particle* p : particle_field ) {
		if (p->life > 0) {
			for (int vert = 0; vert < p->xs.size(); ++vert) {
				float tx = p->xs[vert] / tex->w;
				float ty = p->ys[vert] / tex->h;

				vbuf.push_back(p->xs[vert]);
				vbuf.push_back(p->ys[vert]);
				vbuf.push_back(tx);
				vbuf.push_back(ty);
				vbuf.push_back(p->life);
				ibuf.push_back(index++);
			}

			ibuf.push_back(RESTART);
		}
	}

	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, vbuf.size() * sizeof(float), &(vbuf[0]), GL_STATIC_DRAW );

	if ( glGetError() != GL_NO_ERROR )
		log_error( "Error buffering vbo" );

	// bind the indices to ibo
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, ibuf.size() * sizeof( unsigned int ), &(ibuf[0]), GL_STATIC_DRAW );

	if ( glGetError() != GL_NO_ERROR )
		log_error( "Error bufferin ibo" );

	glBindVertexArray( vao );

	if ( glGetError() != GL_NO_ERROR )
		log_error( "Error while binding VAO" );

	glPrimitiveRestartIndex( RESTART );

	shader->enable_vertex_pointer();
	shader->enable_tex_coord_pointer();
	((FLSmokeBackgroundShader*)shader)->enable_life_pointer();

		glBindBuffer( GL_ARRAY_BUFFER, vbo );
		shader->set_vertex_pointer( VERT_SIZE * sizeof(float), NULL );
		shader->set_tex_coord_pointer( VERT_SIZE * sizeof(float), (const void*) (2 * sizeof(float)) );
		((FLSmokeBackgroundShader*)shader)->set_life_pointer( VERT_SIZE * sizeof(float), (const void*) (4 * sizeof(float)) );

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
		glBindVertexArray( 0 );
}

void FLDistortionSurface::render() {
	update_particle_field();
	update_buffers();
	FLTexturedSurface::render();
}

