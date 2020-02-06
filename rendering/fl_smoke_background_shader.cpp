/*
 * 	fl_framebuffer_shader.cpp
 *
 */

#include <SDL2/SDL.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

#include "../logging/logging.h"
#include "renderer.h"
#include "fl_smoke_background_shader.h"
#include "particles/particle.h"

#define NUM_PARTICLES 128

FLSmokeBackgroundShader::FLSmokeBackgroundShader() : FLTexturedRectShader() {
};

FLSmokeBackgroundShader::~FLSmokeBackgroundShader() {
	clear_particle_field();
}

void FLSmokeBackgroundShader::clear_particle_field() {
	for (int i = 0; i < particle_field.size(); ++i) {
		if (particle_field[i] != nullptr)
			delete particle_field[i];
		if (alt_particle_field[i] != nullptr)
			delete alt_particle_field[i];
	}

	particle_field.clear();
	alt_particle_field.clear();
}

bool FLSmokeBackgroundShader::create_program( std::string program_name ) {
	log_progress( "FLSmokeBackgroundShader::create_program" );

	if ( !FLTexturedRectShader::create_program( program_name ) )
		return false;

	return true;
}

void FLSmokeBackgroundShader::render( GLuint vao, unsigned int num_indices ) {
	update_particle_field();

	glBindVertexArray( vao );
	glDrawElements( GL_TRIANGLE_FAN, num_indices, GL_UNSIGNED_INT, NULL );
	glBindVertexArray( 0 );
}

void FLSmokeBackgroundShader::init_particle_field() {
	// init random num generator
	srand( time(NULL) );	

	clear_particle_field();

	for ( int i = 0; i < NUM_PARTICLES; ++i ) {
		particle_field.push_back( new fl_particle{0.f, 0.f, 60} );
		alt_particle_field.push_back( new fl_particle{0.f, 0.f, 60} );
	}

	particle_float_locations = std::vector<float*>( NUM_PARTICLES * 2, nullptr );
	alt_particle_float_locations = std::vector<float*>( NUM_PARTICLES * 2, nullptr );

	for ( unsigned int particle = 0; particle < particle_field.size(); ++particle ) {
		particle_field[particle]->x = (float)(rand() % 768);
		particle_field[particle]->y = (float)(rand() % 512);

		// we set these pointers to avoid ugly/inefficient math later
		// when passing these values to the shader
		// this works as long as the lifetime of the particles is the
		// lifetime of the particle shader (ie. we re-use particles and
		// don't call new/delete)
		particle_float_locations[particle*2] = &(particle_field[particle]->x);
		particle_float_locations[(particle*2) + 1] = &(particle_field[particle]->y);
		alt_particle_float_locations[particle*2] = &(alt_particle_field[particle]->x);
		alt_particle_float_locations[(particle*2) + 1] = &(alt_particle_field[particle]->y);
	}
}


void FLSmokeBackgroundShader::update_particle_field() {
	float left, right, top, bot;
	
	for ( int i = 0; i < particle_field.size(); ++i ) {
		// do some processing, taking care not to mix addresses
		// between the two buffers
		alt_particle_field[i]->x = particle_field[i]->x;
		alt_particle_field[i]->y = particle_field[i]->y;
		alt_particle_field[i]->life = particle_field[i]->life - 1;

		if (alt_particle_field[i]->life == 0) {
			alt_particle_field[i]->y = 0;
			alt_particle_field[i]->life = 60;
		}

		alt_particle_field[i]->y += 5;

	}

	//swap the updated and old buffer
	std::swap(alt_particle_field, particle_field);

	// make sure we are always pointing to the currently used buffer
	std::swap(alt_particle_float_locations, particle_float_locations);
}

