/*
 *	close_rendering.cpp
 *
 *	Functionality to clean up rendere when program is finished.
 *
 */

#include "../logging/logging.h"
#include "rendering.h"

void Renderer::close() {
	log_progress("Closing rendering libraries");
	glDeleteProgram( gProgramID );

	SDL_DestroyWindow( window );
	window = NULL;

	SDL_Quit();
}

