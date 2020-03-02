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
		void render( GLuint vao, unsigned int num_indices );
	protected:
		GLint time_location;		
};


#endif

