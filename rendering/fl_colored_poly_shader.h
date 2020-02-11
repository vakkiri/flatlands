/*
 * 	fl_colored_poly_shader.h
 *
 * 	Wrapper for shaders which worth with polygons consisting of
 * 	coloured vertices.
 *
 */

#ifndef FL_COLORED_POLY_SHADER_H_
#define FL_COLORED_POLY_SHADER_H_

#include <string>
#include "fl_polygon_shader.h"

class FLColoredPolyShader : public virtual FLPolygonShader {
	public:
		FLColoredPolyShader();

		virtual bool create_program( std::string program_name );
		virtual void render( GLuint vao, unsigned int num_indices );

		void set_color_pointer( GLsizei stride, const GLvoid* data );
		void enable_color_pointer();
	protected:
		GLint color_location;
};

#endif

