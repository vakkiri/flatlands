/*
 * 	fl_textured_rect_shader.h
 *
 * 	A shader class for drawing textured rects.
 *
 */

#ifndef FL_TEXTURED_RECT_SHADER_H_
#define FL_TEXTURED_RECT_SHADER_H_

#include "fl_static_rect_shader.h"

class FLTexturedRectShader : FLStaticRectShader {
	public:
		FLTexturedRectShader();
		FLTexturedRectShader( std::string program_name );
		virtual bool create_program( std::string program_name );
		virtual void draw_rects();

		void set_tex_unit( GLuint unit );
		void set_tex_coord_pointer( GLsizei stride, const GLvoid* data );
	protected:
		GLint intex_coord_location;
		GLint tex_unit_location;
};

#endif

