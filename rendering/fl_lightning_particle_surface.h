/*
 * 	fl_lightning_particle_surface.h
 *
 * 	Surface for the lightning particle system.
 *
 */

#ifndef FL_LIGHTNING_PARTICLE_SURFACE_H_
#define FL_LIGHTNING_PARTICLE_SURFACE_H_

#include "fl_particle_surface.h"

class FLLightningParticleSurface {
	public:
		FLLightningParticleSurface();

		virtual void update_buffers();

		virtual void update_particle_field();
};

#endif

