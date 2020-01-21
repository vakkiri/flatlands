/*
 * 	fl_framebuffer_shader.h
 *
 *	A shader applied to the framebuffer after rendering.
 */

#ifndef FL_FRAMEBUFFER_SHADER_H_
#define FL_FRAMEBUFFER_SHADER_H_

#include "fl_textured_rect_shader.h"

class FLFramebufferShader : public virtual FLTexturedRectShader {
	public:
		FLFramebufferShader();

		virtual bool create_program( std::string program_name );

		void set_time( float time );
		void set_dx( float dx );
		void set_dy( float dy );
		void set_radius( float radius );
		void render( GLuint vao, unsigned int num_indices );
	protected:
		GLint time_location;		
		GLint dx_location;		
		GLint dy_location;		
		GLint radius_location;		
};


#endif

