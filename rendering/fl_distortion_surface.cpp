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

#define NUM_PARTICLES 5096
#define PARTICLE_SIZE 4
#define PARTICLE_LIFE 60
#define VERT_SIZE 5

#define RESTART 0xFFFF

FLDistortionSurface::FLDistortionSurface() : FLTexturedSurface() {
	num_verts = NUM_PARTICLES * 4;		// square particles -> 4 verts
	num_indices = NUM_PARTICLES * 5; 	// 4 + RESTART
	vbuf = new float[num_verts * VERT_SIZE];	
	ibuf = new unsigned int[num_indices];

	init_particle_field();
}

FLDistortionSurface::~FLDistortionSurface() {
	clear_particle_field();
	delete [] vbuf;
	delete [] ibuf;
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
	particle_field[next_loc]->x = x;
	particle_field[next_loc]->y = y;
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
		particle_field.push_back( new fl_particle{ 0.f, 0.f, 0.f, 0.f, 0 } );
		alt_particle_field.push_back( new fl_particle{ 0.f, 0.f, 0.f, 0.f, 0 } );
	}

	add_particle( (float)(rand() % 768), (float)(rand() % 512) );

	// initialize indices
	for ( int i = 0; i < NUM_PARTICLES; ++i ) {
		ibuf[i*5] = i*4;
		ibuf[(i*5)+1] = (i*4) + 1;
		ibuf[(i*5)+2] = (i*4) + 2;
		ibuf[(i*5)+3] = (i*4) + 3;
		ibuf[(i*5)+4] = RESTART;
	}

	for ( int i = 0; i < NUM_PARTICLES; ++i ) {
		ibuf[(i*5)] = i * 4;
		ibuf[(i*5)+1] = (i * 4) + 1;
		ibuf[(i*5)+2] = (i * 4) + 2;
		ibuf[(i*5)+3] = (i * 4) + 3;
		ibuf[(i*5)+4] = RESTART;
	}

}

void FLDistortionSurface::update_particle_field() {
	for ( int i = 0; i < NUM_PARTICLES; ++i ) {
		alt_particle_field[i]->x = particle_field[i]->x;
		alt_particle_field[i]->y = particle_field[i]->y;
		alt_particle_field[i]->life = particle_field[i]->life - 1;

		if ( alt_particle_field[i]->life == 0 ) {
			float newvx1 = alt_particle_field[i]->vx - 0.1;
			float newvx2 = alt_particle_field[i]->vx + 0.1;
			float newvy1 = alt_particle_field[i]->vy - 0.1;
			float newvy2 = alt_particle_field[i]->vy + 0.1;

			add_particle(alt_particle_field[i]->x, alt_particle_field[i]->y, newvx1, newvy1 );
			add_particle(alt_particle_field[i]->x, alt_particle_field[i]->y, newvx2, newvy2 );
			alt_particle_field[i]->life = PARTICLE_LIFE;
		}

		alt_particle_field[i]->y += alt_particle_field[i]->vy;
		alt_particle_field[i]->x += alt_particle_field[i]->vx;

		if (alt_particle_field[i]->y >= 512.f || alt_particle_field[i]->y <= 0)
			alt_particle_field[i]->vy *= -1;
		if (alt_particle_field[i]->x >= 768.f || alt_particle_field[i]->x <= 0)
			alt_particle_field[i]->vx *= -1;
	}

	std::swap( alt_particle_field, particle_field );
}

void FLDistortionSurface::update_buffers() {
	unsigned int step = VERT_SIZE * 4;	// vert size * 4 per quad

	float tleft;
	float tright;
	float ttop;
	float tbot;

	for ( int i = 0; i < particle_field.size(); i++ ) {
		tleft = particle_field[i]->x / tex->w;
		tright = tleft + (float(PARTICLE_SIZE) / tex->w);
		ttop = particle_field[i]->y / tex->h;
		tbot = ttop + (float(PARTICLE_SIZE) / tex->h);

		// top left
		vbuf[i * step] = particle_field[i]->x;
		vbuf[(i * step)+1] = particle_field[i]->y;

		// top right
		vbuf[(i * step)+5] = particle_field[i]->x + PARTICLE_SIZE;
		vbuf[(i * step)+6] = particle_field[i]->y;

		// bottom right
		vbuf[(i * step)+10] = particle_field[i]->x + PARTICLE_SIZE;
		vbuf[(i * step)+11] = particle_field[i]->y + PARTICLE_SIZE;

		// bottom left
		vbuf[(i * step)+15] = particle_field[i]->x;
		vbuf[(i * step)+16] = particle_field[i]->y + PARTICLE_SIZE;

		// tex coords
		vbuf[(i * step) + 2] = tleft;
		vbuf[(i * step) + 3] = ttop;
		vbuf[(i * step) + 7] = tright;
		vbuf[(i * step) + 8] = ttop;
		vbuf[(i * step) + 12] = tright;
		vbuf[(i * step) + 13] = tbot;
		vbuf[(i * step) + 17] = tleft;
		vbuf[(i * step) + 18] = tbot;

		// life
		vbuf[(i * step) + 4] = particle_field[i]->life;
		vbuf[(i * step) + 9] = particle_field[i]->life;
		vbuf[(i * step) + 14] = particle_field[i]->life;
		vbuf[(i * step) + 19] = particle_field[i]->life;

		// indices are set on intialization since they don't change
	}

	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, num_verts * VERT_SIZE * sizeof(float), vbuf, GL_STATIC_DRAW );

	if ( glGetError() != GL_NO_ERROR )
		log_error( "Error buffering vbo" );

	// bind the indices to ibo
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof( unsigned int ), ibuf, GL_STATIC_DRAW );

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

