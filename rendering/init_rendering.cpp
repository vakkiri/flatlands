/*
 *	init_rendering.cpp
 *
 */

#include "../logging/logging.h"
#include "fl_shader.h"
#include "rendering.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool Renderer::init_shaders() {
	cur_shader = new FLShader("basic_shader");
	cur_shader->create_program();

	// VBO
	GLfloat vertexData[] = 
	{
		0.f, 0.f,
		16.f, 0.f,
		16.f, 16.f,
		0.f, 16.f
	};

	// IBO
	GLuint indexData[] = { 0, 1, 2, 3 };

	// Create VBO
	glGenBuffers( 1, &gVBO );
	glBindBuffer( GL_ARRAY_BUFFER, gVBO );
	glBufferData( GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW );
	
	// Create IBO
	glGenBuffers( 1, &gIBO );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW );

	return true;
}

bool Renderer::init_gl() {
	log_progress("Initializing OpenGL");

	// Set viewport
	glViewport( 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT );

	// Initialize projection matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0);

	// Initialize modelview matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glClearColor( 0.1f, 0.f, 0.f, 0.05f );
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );
	glDisable( GL_DEPTH_TEST );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	GLenum error = glGetError();

	if ( error != GL_NO_ERROR ) {
		log_error("Could not initialize OpenGL");
		return false;
	}

	return true;
}

bool Renderer::init_window() {
	log_progress("Creating window");

	window = SDL_CreateWindow( "shift", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
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

