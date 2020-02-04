/*
 * 	fl_framebuffer_shader.cpp
 *
 */

#include <SDL2/SDL.h>
#include "../logging/logging.h"
#include "renderer.h"
#include "fl_smoke_background_shader.h"

FLSmokeBackgroundShader::FLSmokeBackgroundShader() : FLTexturedRectShader() {};

bool FLSmokeBackgroundShader::create_program( std::string program_name ) {
	log_progress( "FLSmokeBackgroundShader::create_program" );

	particle_field = std::vector<std::vector<float>>( Renderer::getInstance().get_screen_height(), std::vector<float>(Renderer::getInstance().get_screen_width(), 0.f));

	if ( !FLTexturedRectShader::create_program( program_name ) )
		return false;

	time_location = glGetUniformLocation( program_id, "time" );

	if ( time_location == -1 )
		return false;

	return true;
}

void FLSmokeBackgroundShader::set_time( float time ) { glUniform1f( time_location, time ); }

void FLSmokeBackgroundShader::render( GLuint vao, unsigned int num_indices ) {
	set_time((float)SDL_GetTicks() / 1000.f);
	FLTexturedRectShader::render( vao, num_indices );
}

