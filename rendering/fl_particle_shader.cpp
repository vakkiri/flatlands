/*
 *	fl_particle_shader.cpp
 *
 */

#include "../logging/logging.h"
#include "fl_particle_shader.h"

FLParticleShader::FLParticleShader() : FLPolygonShader() {};

bool FLParticleShader::create_program( std::string program_name ) {
	log_progress( "FLParticleShader::create_program" );

	if ( !FLPolygonShader::create_program( program_name ) )
		return false;

	life_location = glGetAttribLocation( program_id, "life" );

	if ( life_location == -1 )
		log_warning( "Could not find life in shader" );

	return true;
}

void FLParticleShader::set_life_pointer( GLsizei stride, const GLvoid* data ) {
	glVertexAttribPointer( life_location, 1, GL_FLOAT, GL_FALSE, stride, data );
	if ( glGetError() != GL_NO_ERROR )
			log_warning( "Error setting life pointer." );
}

void FLParticleShader::enable_life_pointer() {
	glEnableVertexAttribArray( life_location );
	if ( glGetError() != GL_NO_ERROR )
			log_warning( "Error enabling life pointer." );
}

void FLParticleShader::render( GLuint vao, unsigned int num_indices ) {
	if ( num_indices > 0 ) {
        glBindVertexArray( vao );
        glDrawElements( GL_TRIANGLE_FAN, num_indices, GL_UNSIGNED_INT, NULL );
        glBindVertexArray( 0 );
    }
}


