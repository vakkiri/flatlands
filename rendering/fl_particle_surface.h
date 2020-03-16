/*
 * 	fl_particle_surface.h
 *
 * 	Surface containing a field of particles to draw.
 *
 */

#ifndef FL_PARTICLE_SURFACE_H
#define FL_PARTICLE_SURFACE_H

#include "rendered_surface.h"
#include "particles/particle.h"

class FLParticleShader;

class FLParticleSurface : public FLRenderedSurface { 
	public:
		FLParticleSurface(unsigned int num_particles, unsigned int particle_life, float particle_size);

		virtual void update_buffers();
		virtual void render();

		virtual void init_particle_field();
		virtual void update_particle_field() = 0;
		virtual void add_particle( float x, float y );
		virtual void add_particle( float x, float y, float vx, float vy );
	protected:
		std::vector<fl_particle> particle_field;
		std::vector<fl_particle> alt_particle_field;
		unsigned int next_loc;
		unsigned int num_particles;
		unsigned int particle_life;
		float particle_size;
		FLParticleShader* shader;
};

#endif

