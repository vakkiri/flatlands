/*
 * 	fl_textured_rect_shader.cpp
 *
 */

#include "fl_text_shader.h"
#include "../logging/logging.h"
#include "texture.h"

FLTextShader::FLTextShader() : FLPolygonShader() {}

bool FLTextShader::create_program(std::string program_name) {
	log_progress("FLTextShader::create_program");

	if (!FLPolygonShader::create_program(program_name))
		return false;

	intex_coord_location = glGetAttribLocation(program_id, "inTexCoord");

	if (intex_coord_location == -1) {
		log_error("Could not find inTexCoord in shader");
		return false;
	}

	tex_unit_location = glGetUniformLocation(program_id, "texUnit");

	if (tex_unit_location == -1) {
		log_error("Could not find inTexCoord in shader");
		return false;
	}

	color_location = glGetAttribLocation(program_id, "inColor");

	if (color_location == -1) {
		log_error("Could not find inColor in shader");
		return false;
	}

	bind();

	set_tex_unit(0);

	if (glGetError() != GL_NO_ERROR)
		log_error("Error setting texture unit");

	return true;
}

void FLTextShader::set_tex_coord_pointer(GLsizei stride, const GLvoid *data) {
	glVertexAttribPointer(intex_coord_location, 2, GL_FLOAT, GL_FALSE, stride, data);
}

void FLTextShader::set_color_pointer(GLsizei stride, const GLvoid *data) {
	glVertexAttribPointer(color_location, 4, GL_FLOAT, GL_FALSE, stride, data);
}

void FLTextShader::set_tex_unit(GLuint unit) {
	glUniform1i(tex_unit_location, unit);
}

void FLTextShader::enable_tex_coord_pointer() {
	glEnableVertexAttribArray(intex_coord_location);
}

void FLTextShader::render(GLuint vao, unsigned int num_indices) {
	if (num_indices > 0) {
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLE_FAN, num_indices, GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
	}
}

GLint FLTextShader::get_id() { return program_id; }
