/*
 *	init_rendering.cpp
 *
 */

#include <iostream>
#include <glm/ext.hpp>

#include "../logging/logging.h"
#include "fl_rect_shader.h"
#include "rendering.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool Renderer::init_shaders() {
	GLenum error;

	log_progress("Initializing shaders");
	cur_shader = new FLRectShader("basic_shader");
	cur_shader->create_program();
	cur_shader->bind();
	
	if ( (error = glGetError()) != GL_NO_ERROR ) {
		log_error("Could not create shader");
		std::cout << "Error: " << error;
		return false;
	}

	//initialize shader matrices
	cur_shader->set_projection( glm::ortho<GLfloat>( 0.0, (float)screen_width, (float)screen_height, 0.0, 1.0, -1.0) );
	cur_shader->update_projection();

	if ( (error = glGetError()) != GL_NO_ERROR ) {
		log_error("Could not set shader projection matrix");
		std::cout << "Error: " << error;
		return false;
	}

	cur_shader->set_modelview( glm::mat4() );
	cur_shader->update_modelview();

	if ( (error = glGetError()) != GL_NO_ERROR ) {
		log_error("Could not set shader modelview matrix");
		std::cout << "Error: " << error;
		return false;
	}
	// VBO
	GLfloat vertexData[] = 
	{
		// rect 1
		0, 0.f,
		16.f, 0.f,
		16.f, 16.f,
		0.f, 16.f,

		// rect 2
		32, 32,
		64, 32,
		64, 64,
		32, 64
	};

	// IBO
	GLuint indexData[] = { 0, 1, 2, 3, 4, 5, 6, 7};

	// Create VBO
	glGenBuffers( 1, &gVBO );
	glBindBuffer( GL_ARRAY_BUFFER, gVBO );
	glBufferData( GL_ARRAY_BUFFER, 2 * 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW );
	
	// Create IBO
	glGenBuffers( 1, &gIBO );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, 2 * 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW );

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

