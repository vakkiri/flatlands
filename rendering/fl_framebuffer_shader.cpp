/*
 * 	fl_framebuffer_shader.cpp
 *
 */

#include <SDL2/SDL.h>
#include "../logging/logging.h"
#include "fl_framebuffer_shader.h"

FLFramebufferShader::FLFramebufferShader() : FLTexturedRectShader() {};

bool FLFramebufferShader::create_program( std::string program_name ) {
	log_progress( "FLFramebufferShader::create_program" );

	if ( !FLTexturedRectShader::create_program( program_name ) )
		return false;



	time_location = glGetUniformLocation( program_id, "time" );
	dx_location = glGetUniformLocation( program_id, "dx" );
	dy_location = glGetUniformLocation( program_id, "dy" );
	radius_location = glGetUniformLocation( program_id, "effectRadius" );

	if ( time_location == -1 || dx_location == -1 || dy_location == -1 || radius_location == -1 )
		return false;

	return true;
}

void FLFramebufferShader::set_time( float time ) { glUniform1f( time_location, time ); }

void FLFramebufferShader::set_dx( float dx ) { glUniform1f( dx_location, dx ); }

void FLFramebufferShader::set_dy( float dy ) { glUniform1f( dy_location, dy ); }

// for set radius we pass the square of the radius to avoid taking square roots when calculating distance in the shader
void FLFramebufferShader::set_radius( float radius ) { glUniform1f( radius_location, radius * radius ); }

void FLFramebufferShader::render( GLuint vao, unsigned int num_indices ) {
	set_time((float)SDL_GetTicks() / 1000.f);
	FLTexturedRectShader::render( vao, num_indices );
}

