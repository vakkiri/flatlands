/*
 * 	fl_textured_rect_shader.h
 *
 * 	A shader class for drawing textured rects.
 *
 */

#ifndef FL_TEXTURED_RECT_SHADER_H_
#define FL_TEXTURED_RECT_SHADER_H_

#include <string>
#include "fl_static_rect_shader.h"

struct tile;
struct texture;

class FLTexturedRectShader : public FLStaticRectShader {
	public:
		FLTexturedRectShader();
		FLTexturedRectShader( std::string program_name );
		virtual bool create_program( std::string program_name );
		virtual void draw_rects();

		virtual void set_geometry( std::vector<tile*>& tiles );
		void set_tex( texture *tex );
		void set_tex( std::string name );
		void set_tex_unit( GLuint unit );
		void set_tex_coord_pointer( GLsizei stride, const GLvoid* data );
	protected:
		GLuint tex_vbo;
		GLint intex_coord_location;
		GLint tex_unit_location;
		texture *tex;
};

#endif

