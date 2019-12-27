/*
	main.cpp

	Start the game! Minimal code should be here.
*/

#include <SDL2/SDL.h>

#include "logging/logging.h"
#include "rendering/rendering.h"
#include "common/basic_types.h"

void main_loop() {
	Renderer& renderer = Renderer::getInstance();

	bool quit = false;

	SDL_Event e;
	SDL_StartTextInput();

	while ( !quit ) {
		while( SDL_PollEvent( &e ) != 0 ) {
			quit = e.type == SDL_QUIT;

			//handle input
		}

		renderer.render_and_swap();
	}
}

int main( int argc, char* args[] ) {
	Renderer& renderer = Renderer::getInstance();
	log_progress("Starting shift");

	if ( renderer.init() ) {
		main_loop();
	}
	else {
		log_error("Could not initialize rendering engine.");
	}

	renderer.close();
	log_progress("Closing shift");
	return 0;
}

