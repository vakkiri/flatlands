/*
 * 	fl_distortion_surface.cpp
 *
 */

#include <SDL2/SDL.h>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>

#include "fl_particle_surface.h"

#include "../logging/logging.h"
#include "particles/particle.h"
#include "renderer.h"
#include "texture.h"
#include "textured_object.h"


FLParticleSurface::FLParticleSurface( unsigned int num_particles, unsigned int particle_life, float particle_size ) {
	this->num_particles = num_particles;
	this->particle_life = particle_life;
	this->particle_size = particle_size;
	init_particle_field();
}

void FLParticleSurface::add_particle( float x, float y ) {
	add_particle(	x, 
			y, 
			0, 
			0);
}

void FLParticleSurface::add_particle( float x, float y, float vx, float vy ) {
	unsigned int verts = 4;

	particle_field[next_loc].xs.clear();
	particle_field[next_loc].ys.clear();

	// We default to creating a quad with 4 vertices
	// v1
	particle_field[next_loc].xs.push_back(x);
	particle_field[next_loc].ys.push_back(y);

	// v2
	particle_field[next_loc].xs.push_back( x + particle_size );
	particle_field[next_loc].ys.push_back(y);

	// v3
	particle_field[next_loc].xs.push_back( x + particle_size );
	particle_field[next_loc].ys.push_back( y + particle_size );

	// v4
	particle_field[next_loc].xs.push_back(x);
	particle_field[next_loc].ys.push_back( y + particle_size );

	particle_field[next_loc].life = particle_life;
	particle_field[next_loc].vx = vx;
	particle_field[next_loc].vy = vy;

	if ( ++next_loc >= num_particles )
		next_loc = 0;
}

void FLParticleSurface::init_particle_field() {
	srand( time(NULL) );

	particle_field = std::vector<fl_particle>(num_particles);

	for ( int i = 0; i < num_particles; ++i )
		add_particle(0, 0);

	alt_particle_field = particle_field;
}


