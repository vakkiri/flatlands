/*
 * 	fl_textured_rect_shader.cpp
 *
 */

#include "../logging/logging.h"
#include "fl_textured_rect_shader.h"
#include "texture.h"

#define RESTART 0xFFFF

FLTexturedRectShader::FLTexturedRectShader() : FLPolygonShader() {};

bool FLTexturedRectShader::create_program( std::string program_name ) {
	log_progress( "FLTexturedRectShader::create_program");

	if ( !FLPolygonShader::create_program( program_name ) )
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

	bind();

	set_tex_unit(0);

	if ( glGetError() != GL_NO_ERROR )
		log_error( "Error setting texture unit" );

	return true;
}

void FLTexturedRectShader::set_tex_coord_pointer( GLsizei stride, const GLvoid* data ) {
	glVertexAttribPointer( intex_coord_location, 2, GL_FLOAT, GL_FALSE, stride, data );
}

void FLTexturedRectShader::set_tex_unit( GLuint unit ) {
	glUniform1i( tex_unit_location, unit );
}

void FLTexturedRectShader::enable_tex_coord_pointer() {
	glEnableVertexAttribArray( intex_coord_location );
}

void FLTexturedRectShader::render( GLuint vao, unsigned int num_indices ) {
	glBindVertexArray( vao );
	glDrawElements( GL_TRIANGLE_FAN, num_indices, GL_UNSIGNED_INT, NULL );
	glBindVertexArray( 0 );
}
