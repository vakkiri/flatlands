/*
 * 	main.cpp
 *
 * 	Initialization and main game loop.
 *
 */

#include <SDL2/SDL.h>
#include <iostream>

#include "audio/fl_audio.h"
#include "common/common.h"
#include "input/input_handler.h"
#include "logging/logging.h"
#include "net/fl_net.h"
#include "rendering/renderer.h"
#include "resources/fl_resources.h"
#include "tilemap/tilemap.h"
#include "ui/fl_ui_manager.h"
#include "world/player/player.h"
#include "world/world_environment.h"

#define TARGET_FPS 60
#define MS_PER_FRAME (1000 / TARGET_FPS)

void main_loop() {
	Renderer& renderer = Renderer::getInstance();
	FLWorldEnvironment& world = FLWorldEnvironment::getInstance();
	FLInputHandler& input_handler = FLInputHandler::getInstance();
	FLUIManager& ui_manager = FLUIManager::getInstance();

	// initialization	
	renderer.init_surface_textures();
	input_handler.init();
	ui_manager.init();	

	// Test objects...
	FLTilemap tilemap(1024, 1024, 16);
	world.set_tilemap(&tilemap);
	world.set_player(new FLPlayer);

	world.load_next_level();
	// End of test objects

	bool quit = false;

	unsigned int start_time = 0;
	unsigned int end_time = 0;

	while ( !quit ) {
		start_time = SDL_GetTicks();
		end_time = start_time + MS_PER_FRAME;

		quit = input_handler.input_loop();

		world.update();

		renderer.render_and_swap();

		fl_update_server();
		fl_update_client();

		while ( SDL_GetTicks() < end_time ) {
			fl_update_server();
			fl_update_client();
		}
	}
}

int main( int argc, char* args[] ) {
	Renderer& renderer = Renderer::getInstance();
	FLResources& resources = FLResources::getInstance();
	log_progress("Starting shift");

	if ( renderer.init() && init_audio() && fl_init_net() ) {
		if ( resources.init() ) {
			fl_start_server();
			fl_start_client();
			main_loop();
		}
		else {
			log_error( "Could not initialize resources." );
		}
	}
	else {
		log_error( "Could not initialize rendering engine." );
	}

	renderer.close();
	resources.close();
	log_progress("Closing shift");
	return 0;
}

