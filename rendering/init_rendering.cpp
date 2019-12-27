/*
 *	init_rendering.cpp
 *
 */

#include <iostream>
#include <glm/ext.hpp>

#include "../logging/logging.h"
#include "../common/basic_types.h"
#include "rendering.h"

#define PRIMITIVE_RESTART 65535

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool Renderer::init_shaders() {
	GLenum error;

	log_progress("Initializing shaders");
	static_rect_shader.create_program("instanced_rect_shader");
	static_rect_shader.bind();
	
	if ( (error = glGetError()) != GL_NO_ERROR ) {
		log_error("Could not create shader");
		std::cout << "Error: " << error;
		return false;
	}

	std::vector<rect> rects;
	rect r1;
	r1.x = 0.f;
	r1.y = 0.f;
	r1.w = 16.f;
	r1.h = 16.f;
	rect r2;
	r2.x = 40.f;
	r2.y = 50.f;
	r2.w = 16.f;
	r2.h = 16.f;
	rect r3;
	r3.x = 400.f;
	r3.y = 200.f;
	r3.w = 16.f;
	r3.h = 16.f;

	rects.push_back(r1);
	rects.push_back(r2);
	rects.push_back(r3);
	
	static_rect_shader.set_geometry(rects);

	//initialize shader projection matrix
	static_rect_shader.set_projection( glm::ortho<GLfloat>( 0.0, screen_width, screen_height, 0.0, 1.0, -1.0 ) );
	static_rect_shader.update_projection();

	//initialize shader camera matrix
	static_rect_shader.set_camera( glm::mat4(1.0) );
	static_rect_shader.update_camera();


	if ( (error = glGetError()) != GL_NO_ERROR ) {
		log_error("Could not set shader camera matrix");
		std::cout << "Error: " << error;
		return false;
	}

	return true;
}

bool Renderer::init_gl() {
	GLenum error;
	log_progress("Initializing OpenGL");

	// Set viewport
	glViewport( 0.f, 0.f, (float) screen_width, (float) screen_height );

	glClearColor( 0.1f, 0.f, 0.05f, 1.0f );
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );
	glDisable( GL_DEPTH_TEST );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(PRIMITIVE_RESTART);

	if ( (error = glGetError()) != GL_NO_ERROR ) {
		log_error("Could not initialize OpenGL");
		std::cout << "Error: " << error;
		return false;
	}

	return true;
}

bool Renderer::init_window() {
	log_progress("Creating window");

	window = SDL_CreateWindow( "shift", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
	if ( window == nullptr ) {
		log_error( SDL_GetError() );
		return false;
	}

	context = ( SDL_GL_CreateContext( window ) );
	if ( context == NULL ) {
		log_error( SDL_GetError() );
		return false;
	}

	if ( SDL_GL_SetSwapInterval( 1 ) < 0 )
		log_warning( SDL_GetError() );

	return true;
}

bool Renderer::init_sdl() {
	log_progress("Initializing SDL");

	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		log_error( SDL_GetError() );
		return false;
	}
	
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

	return true;
}

bool Renderer::init() {
	log_progress("Initializing rendering engine");
	screen_width = SCREEN_WIDTH;
	screen_height = SCREEN_WIDTH;

	if (!init_sdl()) {
		log_error("Could not initialize SDL");
		return false;
	}
	if (!init_window()) {
		log_error("Could not create window");
		return false;
	}
	if (!init_gl()) {
		log_error("Could not initialize OpenGL");
		return false;
	}
	if (!init_shaders()) {
		log_error("Could not initialize shaders");
		return false;
	}

	return true;
}

