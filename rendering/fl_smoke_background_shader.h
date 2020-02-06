/*
 * 	fl_framebuffer_shader.h
 *
 *	A shader applied to the framebuffer after rendering.
 */

#ifndef FL_SMOKE_BACKGROUND_SHADER_H_
#define FL_SMOKE_BACKGROUND_SHADER_H_

#include <vector>
#include "fl_textured_rect_shader.h"
#include "rendered_surface.h"

struct fl_particle;

class FLSmokeBackgroundShader : public virtual FLTexturedRectShader {
	public:
		FLSmokeBackgroundShader();
		virtual ~FLSmokeBackgroundShader();
		virtual bool create_program( std::string program_name );

		void set_time( float time );
		void render( GLuint vao, unsigned int num_indices );
		virtual void init_particle_field();
		virtual void update_particle_field();
		virtual void clear_particle_field();
	protected:
		std::vector<fl_particle*> particle_field;
		std::vector<fl_particle*> alt_particle_field;
		std::vector<float*> particle_float_locations;
		std::vector<float*> alt_particle_float_locations;
};


#endif

