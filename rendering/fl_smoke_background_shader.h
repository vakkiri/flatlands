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

class FLSmokeBackgroundShader : public FLTexturedRectShader {
	public:
		FLSmokeBackgroundShader();
		virtual bool create_program( std::string program_name );

		void render( GLuint vao, unsigned int num_indices );

		void set_life_pointer( GLsizei stride, const GLvoid* data );
		void enable_life_pointer();
	protected:
		GLint life_location;

};


#endif

