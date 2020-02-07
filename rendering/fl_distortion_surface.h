/*
 * 	fl_distortion_surface.h
 *
 * 	Surface for distorting the framebuffer, ie. with particle
 * 	effects.
 *
 */

#ifndef FL_DISTORTION_SURFACE_H
#define FL_DISTORTION_SURFACE_H

#include "rendered_surface.h"

struct fl_particle;
class FLSmokeBackgroundShader;

class FLDistortionSurface : public FLTexturedSurface { 
	public:
		FLDistortionSurface();
		virtual ~FLDistortionSurface();

		virtual void render();
		virtual void update_buffers();

		virtual void init_particle_field();
		virtual void update_particle_field();
		virtual void clear_particle_field();
		virtual void add_particle( float x, float y );
		virtual void add_particle( float x, float y, float vx, float vy );
	protected:
		std::vector<fl_particle*> particle_field;
		std::vector<fl_particle*> alt_particle_field;
		unsigned int next_loc;
		float *vbuf;		// buffer of all verts
		unsigned int *ibuf;	// buffer of all indices


};

#endif

