/*
 * 	fl_rect_shader.h
 *
 * 	A basis for vertex position-specified geometry shaders.
 *
 */

#ifndef FL_POLYGON_SHADER_H_
#define FL_POLYGON_SHADER_H_

#include "fl_shader.h"

class FLPolygonShader : public virtual FLShader {
	public:
		FLPolygonShader() : FLShader(){};

		virtual bool create_program( std::string program_name );

		void set_vertex_pointer( GLsizei stride, const GLvoid* data );
		void enable_vertex_pointer();
		void disable_vertex_pointer();
	protected:
		GLint vertex_pos_location;
};

#endif 
