/*
 *	fl_rect_shader.cpp
 *
 * 	Basic shader to draw untextured rectangles.
 */

#include "fl_polygon_shader.h"
#include "../logging/logging.h"

bool FLPolygonShader::create_program(std::string program_name) {
	if (!FLShader::create_program(program_name))
		return false;

	vertex_pos_location = glGetAttribLocation(program_id, "vertexPos");
	if (vertex_pos_location == -1) {
		log_error("Could not find variable vertexPos in shader.");
		return false;
	}

	return true;
}

void FLPolygonShader::set_vertex_pointer(GLsizei stride, const GLvoid *data) {
	glVertexAttribPointer(vertex_pos_location, 2, GL_FLOAT, GL_FALSE, stride,
						  data);
}

void FLPolygonShader::enable_vertex_pointer() {
	glEnableVertexAttribArray(vertex_pos_location);
}

void FLPolygonShader::disable_vertex_pointer() {
	glDisableVertexAttribArray(vertex_pos_location);
}
