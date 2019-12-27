/*
 *	close_rendering.cpp
 *
 *	Functionality to clean up rendere when program is finished.
 *
 */

#include "../logging/logging.h"
#include "fl_shader.h"
#include "rendering.h"

void Renderer::close() {
	log_progress("Closing rendering libraries");

	SDL_DestroyWindow( window );
	window = NULL;

	SDL_Quit();
}

