/*
 * 	fl_framebuffer_shader.h
 *
 *	A shader applied to the framebuffer after rendering.
 */

#ifndef FL_SMOKE_BACKGROUND_SHADER_H_
#define FL_SMOKE_BACKGROUND_SHADER_H_

#include <vector>
#include "fl_textured_rect_shader.h"

class FLSmokeBackgroundShader : public virtual FLTexturedRectShader {
	public:
		FLSmokeBackgroundShader();

		virtual bool create_program( std::string program_name );

		void set_time( float time );
		void render( GLuint vao, unsigned int num_indices );

	protected:
		std::vector<std::vector<float>> particle_field;
		GLint time_location;		
};


#endif

