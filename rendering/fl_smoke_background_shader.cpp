/*
 * 	fl_framebuffer_shader.cpp
 *
 */

#include <SDL2/SDL.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

#include "fl_smoke_background_shader.h"

#include "../logging/logging.h"
#include "renderer.h"
#include "particles/particle.h"

FLSmokeBackgroundShader::FLSmokeBackgroundShader() : FLTexturedRectShader() {
};

bool FLSmokeBackgroundShader::create_program( std::string program_name ) {
	log_progress( "FLSmokeBackgroundShader::create_program" );

	if ( !FLTexturedRectShader::create_program( program_name ) )
		return false;

	life_location = glGetAttribLocation( program_id, "life" );

	if ( life_location == -1 )
		log_warning( "Could not find life in shader" );

	return true;
}

void FLSmokeBackgroundShader::render( GLuint vao, unsigned int num_indices ) {
	glBindVertexArray( vao );
	glDrawElements( GL_TRIANGLE_FAN, num_indices, GL_UNSIGNED_INT, NULL );
	glBindVertexArray( 0 );
}

void FLSmokeBackgroundShader::enable_life_pointer() {
	glEnableVertexAttribArray( life_location );
	if (glGetError() != GL_NO_ERROR)
		log_warning( "Error enabling life pointer" );
}

void FLSmokeBackgroundShader::set_life_pointer( GLsizei stride, const GLvoid* data ) {
	glVertexAttribPointer( life_location, 1, GL_FLOAT, GL_FALSE, stride, data );
	if (glGetError() != GL_NO_ERROR)
		log_warning( "Error setting life pointer" );
}
