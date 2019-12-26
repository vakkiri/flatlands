/*
 *	fl_rect_shader.cpp
 *
 * 	Basic shader to draw untextured rectangles.
 */

#include "../logging/logging.h"
#include "fl_rect_shader.h"

bool FLRectShader::create_program(std::string program_name) {
	if ( !FLShader::create_program(program_name) )
		return false;

	vertex_pos_2d_location = glGetAttribLocation( program_id, "LVertexPos2D" );
	if ( vertex_pos_2d_location == -1 ) {
		log_error( "Could not find variable LVertexPos2D in shader." );
		return false;
	}

	return true;
}

bool FLRectShader::create_program() {
	return create_program(name);
}

void FLRectShader::set_vertex_pointer( GLsizei stride, const GLvoid* data ) {
	glVertexAttribPointer( vertex_pos_2d_location, 2, GL_FLOAT, GL_FALSE, stride, data );
}

void FLRectShader::enable_vertex_pointer() {
	glEnableVertexAttribArray( vertex_pos_2d_location );
}

void FLRectShader::disable_vertex_pointer() {
	glDisableVertexAttribArray( vertex_pos_2d_location );
}

