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
#include "components/components.h"
#include "environment/fl_environment.h"
#include "game/fl_game.h"
#include "input/input_handler.h"
#include "logging/logging.h"
#include "net/fl_net.h"
#include "rendering/renderer.h"
#include "resources/fl_resources.h"
#include "tilemap/tilemap.h"
#include "ui/fl_ui_manager.h"
#include "world/player/player.h"

#define TARGET_FPS 60
#define MS_PER_FRAME (1000 / TARGET_FPS)

void main_loop() {
	bool quit = false;
	unsigned int start_time = 0;
	unsigned int end_time = 0;

	Renderer &renderer = Renderer::getInstance();
	FLInputHandler &input_handler = FLInputHandler::getInstance();
	FLUIManager &ui_manager = FLUIManager::getInstance();

	// initialization
	renderer.init_surface_textures();
	input_handler.init();

	// start the game
	FLGame::instance().init();
	FLGame::instance().start();
	ui_manager.init();

	// FIXME this loop should just be in the game class
	while (!quit) {
		start_time = SDL_GetTicks();
		end_time = start_time + MS_PER_FRAME;

		quit = input_handler.input_loop();

		fl_update_components();

		renderer.render_and_swap();

		fl_update_server();
		fl_update_client();

		// TODO: would probably be better to just run server and client in a
		// separate thread
		if (SDL_GetTicks() < end_time) {
			SDL_Delay(end_time - SDL_GetTicks());
		}
	}
}

int main() {
	Renderer &renderer = Renderer::getInstance();
	FLResources &resources = FLResources::getInstance();
	log_progress("Initializing flatlands engine...");

	if (renderer.init() && init_audio() && fl_init_net()) {
		if (resources.init()) {
			fl_start_server();
			fl_start_client();
			main_loop();
		} else {
			log_error("Could not initialize resources.");
		}
	} else {
		log_error("Could not initialize rendering engine.");
	}

	renderer.close();
	resources.close();
	log_progress("Closing flatlands engine.");
	return 0;
}
