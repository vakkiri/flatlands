/*
 *	fl_colored_polygon_shader.cpp
 *
 */

#include "../logging/logging.h"
#include "fl_colored_poly_shader.h"

FLColoredPolyShader::FLColoredPolyShader() : FLPolygonShader() {};

bool FLColoredPolyShader::create_program( std::string program_name ) {
	log_progress( "FLColoredPolyShader::create_program" );

	if ( !FLPolygonShader::create_program( program_name ) )
		return false;

	color_location = glGetAttribLocation( program_id, "vertexColor" );

	if ( color_location == -1 )
		log_warning( "Could not find vertexColor in shader" );

	bind();

	return true;
}

void FLColoredPolyShader::set_color_pointer( GLsizei stride, const GLvoid* data ) {
	glVertexAttribPointer( color_location, 4, GL_FLOAT, GL_FALSE, stride, data );
}

void FLColoredPolyShader::enable_color_pointer() {
	glEnableVertexAttribArray( color_location );
}

void FLColoredPolyShader::render( GLuint vao, unsigned int num_indices ) {
    if ( num_indices > 0 ) { 
        glBindVertexArray( vao );
        glDrawElements( GL_TRIANGLE_FAN, num_indices, GL_UNSIGNED_INT, NULL );
        glBindVertexArray( 0 );
    }
}


