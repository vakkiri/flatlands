/*
 * 	fl_framebuffer_shader.cpp
 *
 */

#include <SDL2/SDL.h>
#include "../logging/logging.h"
#include "fl_angel_shader.h"

FLAngelShader::FLAngelShader() : FLFramebufferShader() {};

bool FLAngelShader::create_program( std::string program_name ) {
	log_progress( "FLAngelShader::create_program" );

	if ( !FLFramebufferShader::create_program( program_name ) )
		return false;



	dx_location = glGetUniformLocation( program_id, "dx" );
	dy_location = glGetUniformLocation( program_id, "dy" );
	radius_location = glGetUniformLocation( program_id, "effectRadius" );

	if ( dx_location == -1 || dy_location == -1 || radius_location == -1 )
		return false;

	return true;
}

void FLAngelShader::set_dx( float dx ) { glUniform1f( dx_location, dx ); }

void FLAngelShader::set_dy( float dy ) { glUniform1f( dy_location, dy ); }

// for set radius we pass the square of the radius to avoid taking square roots when calculating distance in the shader
void FLAngelShader::set_radius( float radius ) { glUniform1f( radius_location, radius * radius ); }

