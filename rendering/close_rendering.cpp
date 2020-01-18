/*
 *	close_rendering.cpp
 *
 *	Functionality to clean up rendere when program is finished.
 *
 */

#include "../logging/logging.h"
#include "fl_shader.h"
#include "renderer.h"
#include "texture.h"

void Renderer::close() {
	log_progress("Closing rendering libraries");
	
	glDeleteFramebuffers( 1, &framebuffer );
	SDL_DestroyWindow( window );
	window = NULL;
	delete framebuffer_texture;

	SDL_Quit();
}

