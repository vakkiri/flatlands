/*
 * 	fl_textured_rect_shader.cpp
 *
 */

#include "../logging/logging.h"
#include "fl_textured_rect_shader.h"

FLTexturedRectShader::FLTexturedRectShader() : FLStaticRectShader() {};

FLTexturedRectShader::FLTexturedRectShader( std::string program_name ) : FLStaticRectShader() {
	create_program( program_name );
	bind();
	set_tex_unit(0);
}

bool FLTexturedRectShader::create_program( std::string program_name ) {
	log_progress( "FLTexturedRectShader::create_program");

	if ( !FLStaticRectShader::create_program( program_name ) )
			return false;

	intex_coord_location = glGetAttribLocation( program_id, "inTexCoord" );

	if ( intex_coord_location == -1 ) {
		log_error( "Could not find inTexCoord in shader" );
		return false;
	}

	tex_unit_location = glGetUniformLocation( program_id, "texUnit" );

	if ( tex_unit_location == -1 ) {
		log_error( "Could not find inTexCoord in shader" );
		return false;
	}

	return true;
}

void FLTexturedRectShader::set_tex_coord_pointer( GLsizei stride, const GLvoid* data ) {
	glVertexAttribPointer( intex_coord_location, 2, GL_FLOAT, GL_FALSE, stride, data );
}

void FLTexturedRectShader::set_tex_unit( GLuint unit ) {
}

void FLTexturedRectShader::draw_rects() {
}
