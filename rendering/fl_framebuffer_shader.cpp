/*
 * 	fl_framebuffer_shader.cpp
 *
 */

#include <SDL2/SDL.h>
#include "../logging/logging.h"
#include "fl_framebuffer_shader.h"

FLFramebufferShader::FLFramebufferShader() : FLTexturedRectShader() {}

bool FLFramebufferShader::create_program( std::string program_name ) {
	log_progress( "FLFramebufferShader::create_program" );

	if ( !FLTexturedRectShader::create_program( program_name ) )
		return false;



	time_location = glGetUniformLocation( program_id, "time" );

	if ( time_location == -1 )
		return false;

	return true;
}

void FLFramebufferShader::set_time( float time ) { glUniform1f( time_location, time ); }

void FLFramebufferShader::render( GLuint vao, unsigned int num_indices ) {
    if ( num_indices > 0 ) {
        set_time((float)SDL_GetTicks() / 1000.f);
        FLTexturedRectShader::render( vao, num_indices );
    }
}

