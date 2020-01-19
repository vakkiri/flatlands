/*
 * 	fl_textured_rect_shader.h
 *
 * 	A shader class for drawing textured rects.
 *
 */

#ifndef FL_TEXTURED_RECT_SHADER_H_
#define FL_TEXTURED_RECT_SHADER_H_

#include <string>
#include "fl_polygon_shader.h"

struct tile;
struct texture;

class FLTexturedRectShader : public FLPolygonShader {
	public:
		FLTexturedRectShader();

		virtual bool create_program( std::string program_name );
		virtual void render( GLuint vao, unsigned int num_idices );

		void set_tex_unit( GLuint unit );
		void set_tex_coord_pointer( GLsizei stride, const GLvoid* data );
		void enable_tex_coord_pointer();
		GLint get_id();
	protected:
		GLint intex_coord_location;
		GLint tex_unit_location;
};

#endif

