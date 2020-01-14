/*
 * 	world_environment.cpp
 *
 */

#include <iostream>
#include <algorithm>

#include "../rendering/renderer.h"
#include "../tilemap/tilemap.h"
#include "../utils/collision_utils.h"
#include "colliding_object.h"
#include "player/player.h"
#include "world_environment.h"

FLTilemap* FLWorldEnvironment::tilemap() { return _tilemap; }

void FLWorldEnvironment::reset_environment() {
	reset_tilemap();

	// clear world objects
	for ( auto object : world_objects ) {
		if ( object != nullptr && object != _player )
			delete object;
	}

	world_objects.clear();

	colliding_objects.clear();

	Renderer::getInstance().clear();

	// Load map
	FLResources::getInstance().load_level(level);
	_tilemap->update_surface();

	// Place player back on scene
	world_objects.push_back( _player );
	Renderer::getInstance().add_to_world( _player );	
	Renderer::getInstance().add_animated_object( _player );	

	reset = false;
}

void FLWorldEnvironment::load_next_level() {
	reset = true;
	level = 0;
}

void FLWorldEnvironment::reset_tilemap() {
	if ( _tilemap != nullptr )
		_tilemap->reset();
}

void FLWorldEnvironment::update() {
	if (reset)
		reset_environment();

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

void FLWorldEnvironment::remove_object( FLWorldObject* object ) {
	for ( int i = 0; i < world_objects.size(); ++i ) {
		if ( world_objects[i] == object )
			world_objects[i] = nullptr;
	}
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

std::vector<FLCollidingObject*> FLWorldEnvironment::get_colliding_objects( FLWorldObject* object ) {
	std::vector<FLCollidingObject*> objects;

	for ( FLCollidingObject* other : colliding_objects ) {
		if ( rect_collision( object, other ) )
			objects.push_back( other );
	}

	return objects;
}

void FLWorldEnvironment::mark_reset() {
	reset = true;
}

