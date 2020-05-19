/*
 * 	world_environment.cpp
 *
 */

#include <iostream>
#include <algorithm>

#include "../common/common.h"
#include "../game/fl_gamestate.h"
#include "../rendering/renderer.h"
#include "../tilemap/tilemap.h"

#include "world_environment.h"

#include "player/player.h"

FLTilemap* FLWorldEnvironment::tilemap() { return _tilemap; }

void FLWorldEnvironment::reset_environment() {
	// XXX this code currently doesn't work properly for net games - net players are
	// world objects and will be deleted but not restored properly
	reset_tilemap();

	Renderer::getInstance().clear();

	// Load map
	FLResources::getInstance().load_level(level);
	_tilemap->update_surface();

	// Place player back on scene
	Renderer::getInstance().add_to_world( _player );	
	Renderer::getInstance().add_to_world( _player->get_weapon() );

	reset = false;
}

void FLWorldEnvironment::load_next_level() {
	reset = true;
	level += 1;
}

void FLWorldEnvironment::reset_tilemap() {
	if ( _tilemap != nullptr )
		_tilemap->reset();
}

void FLWorldEnvironment::update() {
	if ( get_game_state() == FL_GAME_RUNNING ) {
		if (reset)
			reset_environment();
	}
}

void FLWorldEnvironment::set_player( FLPlayer* player ) {
	_player = player;
}

FLPlayer* FLWorldEnvironment::player() {
	return _player;
}

void FLWorldEnvironment::set_tilemap(FLTilemap* tilemap) {
	_tilemap = tilemap;
}

void FLWorldEnvironment::mark_reset() {
	reset = true;
}

