/*
 * 	world_environment.cpp
 *
 */

#include "world_environment.h"
#include "../tilemap/tilemap.h"
#include "player/player.h"

FLTilemap* FLWorldEnvironment::tilemap() { return _tilemap; }

void FLWorldEnvironment::reset_tilemap() {
	if ( _tilemap != nullptr )
		_tilemap->reset();
}

void FLWorldEnvironment::update() {
	_player->update_physics();
	_player->update_animation();
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

bool FLWorldEnvironment::solid_at( float x, float y ) {
	return _tilemap->solid_at( x, y );
}

void FLWorldEnvironment::add_object( FLWorldObject* object ) {
	world_objects.push_back( object );
}

