/*
 * 	main.cpp
 *
 * 	Initialization and main game loop.
 *
 */

#include <SDL2/SDL.h>
#include <iostream>

#include "input/input_handler.h"
#include "logging/logging.h"
#include "rendering/renderer.h"
#include "common/common.h"
#include "resources/fl_resources.h"

#include "tilemap/tilemap.h"
#include "world/player/player.h"
#include "world/world_environment.h"

#define TARGET_FPS 60
#define MS_PER_FRAME (1000 / TARGET_FPS)

void main_loop() {
	Renderer& renderer = Renderer::getInstance();
	FLInputHandler& input_handler = FLInputHandler::getInstance();

	// Test objects...
	FLTilemap tilemap(1024, 1024, 1);
	FLResources::getInstance().load_level(0);
	FLWorldEnvironment::getInstance().set_tilemap(&tilemap);

	for ( int i = 0; i < 50; i++ )
		tilemap.add_tile(i * 16, 256, 16, 16, 0, true);

	tilemap.add_tile(128, 256-32, 16, 16, 0, true);
	tilemap.add_tile(128+16, 256-32, 16, 16, 0, true);
	tilemap.add_tile(128+16, 256-16, 16, 16, 0, true);
	tilemap.add_tile(128+16, 256-48, 16, 16, 0, true);

	tilemap.set_texture( "tiles" );
	tilemap.update_surface();

	FLPlayer player(renderer.get_world_surface());
	player.set_texture( FLResources::getInstance().get_image("neko_idle") );
	player.update_surface();
	// End of test objects

	bool quit = false;
	input_handler.init();

	unsigned int start_time = 0;
	unsigned int end_time = 0;

	while ( !quit ) {
		start_time = SDL_GetTicks();
		end_time = start_time + MS_PER_FRAME;

		quit = input_handler.input_loop();

		// test code -------------------------

		player.update_animation();
		player.update_surface();
		player.update_physics();

		// end of test code -----------------

		renderer.render_and_swap();

		while ( SDL_GetTicks() < end_time );
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

