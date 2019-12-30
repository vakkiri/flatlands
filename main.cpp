/*
	main.cpp

	Start the game! Minimal code should be here.
*/

#include <SDL2/SDL.h>

#include "logging/logging.h"
#include "rendering/renderer.h"
#include "common/common.h"
#include "resources/fl_resources.h"
#include "tilemap/tilemap.h"

void main_loop() {
	Renderer& renderer = Renderer::getInstance();
	FLTilemap tilemap(renderer, 512, 512, 16);
	tilemap.add_tile(16, 16, 16, 16, 0);
	tilemap.add_tile(48, 16, 16, 16, 0);
	tilemap.add_tile(48, 32, 16, 16, 3);
	tilemap.set_texture( "tiles" );
	tilemap.update_surface();
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
	FLResources& resources = FLResources::getInstance();
	log_progress("Starting shift");

	if ( renderer.init() ) {
		if ( resources.init() )
			main_loop();
		else
			log_error( "Could not initialize resources." );
	}
	else {
		log_error( "Could not initialize rendering engine." );
	}

	renderer.close();
	resources.close();
	log_progress("Closing shift");
	return 0;
}

