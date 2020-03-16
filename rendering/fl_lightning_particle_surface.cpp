/*
 * 	fl_lightning_particle_surface.cpp
 *
 */

#include <iostream>

#include "fl_lightning_particle_surface.h"

#include "renderer.h"

#define NUM_PARTICLES 500
#define PARTICLE_LIFE 60
#define PARTICLE_SIZE 2

FLLightningParticleSurface::FLLightningParticleSurface() :
	FLParticleSurface( NUM_PARTICLES, PARTICLE_LIFE, PARTICLE_SIZE ) {

	shader = Renderer::getInstance().get_lightning_shader();
}

void FLLightningParticleSurface::update_particle_field() {
	/* method:
	 *
	 * 1. find a bounding box around all the particles
	 * 2. divide this bounding box into buckets
	 * 3. calculate the amount of energy ("life") in each bucket
	 * 4. diffuse each particle into neighbouring buckets
	 *
	 */
	for ( int i = 0; i < particle_field.size(); ++i ) {
		if ( particle_field[i].life > 0 )
			--particle_field[i].life;
	}
}

