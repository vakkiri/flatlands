/*
 * 	fl_rect_shader.h
 *
 * 	A basic shader for rectangles.
 *
 */

#include "fl_shader.h"

class FLRectShader : public FLShader {
	public:
		FLRectShader() : FLShader(){};
		FLRectShader(std::string name) : FLShader(name){};

		virtual bool create_program(std::string program_name);
		virtual bool create_program();

		void set_vertex_pointer( GLsizei stride, const GLvoid* data );
		void enable_vertex_pointer();
		void disable_vertex_pointer();
	protected:
		GLint vertex_pos_2d_location;
};

