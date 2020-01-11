/*
 * 	world_environment.cpp
 *
 */

#include <algorithm>

#include "../tilemap/tilemap.h"
#include "../utils/collision_utils.h"
#include "colliding_object.h"
#include "player/player.h"
#include "world_environment.h"

FLTilemap* FLWorldEnvironment::tilemap() { return _tilemap; }

void FLWorldEnvironment::reset_tilemap() {
	if ( _tilemap != nullptr )
		_tilemap->reset();
}

void FLWorldEnvironment::update() {
	_player->update_physics();
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

void FLWorldEnvironment::add_colliding_object( FLCollidingObject *object ) {
	colliding_objects.push_back( object );
}

void FLWorldEnvironment::remove_colliding_object( FLCollidingObject *object ) {
	colliding_objects.erase( std::remove( colliding_objects.begin(), colliding_objects.end(), object ), colliding_objects.end() );
}

FLCollidingObject* FLWorldEnvironment::get_colliding_object( FLWorldObject* object ) {
	for ( FLCollidingObject* other : colliding_objects ) {
		if ( rect_collision( object, other ) )
			return other;
	}

	return nullptr;
}

