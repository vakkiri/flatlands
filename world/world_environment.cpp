/*
 * 	world_environment.cpp
 *
 */

#include <iostream>
#include <algorithm>

#include "../custom/angel.h"
#include "../game/fl_gamestate.h"
#include "../rendering/renderer.h"
#include "../tilemap/tilemap.h"
#include "../utils/collision_utils.h"

#include "colliding_object.h"
#include "dynamic_object.h"
#include "interactable_object.h"
#include "world_environment.h"

#include "objects/toggle_tile.h"
#include "player/player.h"

FLTilemap* FLWorldEnvironment::tilemap() { return _tilemap; }

void FLWorldEnvironment::reset_environment() {
	reset_tilemap();

	clear_world_objects();

	// clear special collections
	clear_colliding_objects();
	clear_toggle_tiles();
	angels.clear();

	// clear standard collections
	clear_dynamic_objects();

	Renderer::getInstance().clear();

	// Load map
	FLResources::getInstance().load_level(level);
	_tilemap->update_surface();

	// Place player back on scene
	Renderer::getInstance().add_to_world( _player );	
	Renderer::getInstance().add_animated_object( _player );	

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

		update_dynamic_objects();
		_player->update_physics();
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

bool FLWorldEnvironment::solid_at( float x, float y ) {
	return _tilemap->solid_at( x, y );
}

void FLWorldEnvironment::remove_angel( NVAngel *angel ) {
	angels.erase( std::remove( angels.begin(), angels.end(), angel ), angels.end() );
}

std::vector<NVAngel*>* FLWorldEnvironment::get_angels() {
	return &angels;
}

void FLWorldEnvironment::add_interactable_object( FLInteractableObject *object ) {
	interactable_objects.push_back( object );
}

void FLWorldEnvironment::remove_interactable_object( FLInteractableObject *object ) {
	interactable_objects.erase( std::remove( interactable_objects.begin(), interactable_objects.end(), object ), interactable_objects.end() );
}

FLCollidingObject* FLWorldEnvironment::find_colliding_object( FLWorldObject* object ) {
	std::vector<FLCollidingObject*>& colliding_objects = get_colliding_objects();

	for ( FLCollidingObject* other : colliding_objects ) {
		if ( rect_collision( object, other ) )
			return other;
	}

	return nullptr;
}

void FLWorldEnvironment::interact( FLWorldObject* object ) {
	for ( FLInteractableObject* other : interactable_objects ) {
		if ( rect_collision( object, other ) ) {
			other->interact_with();
			break;
		}
	}
}

std::vector<FLCollidingObject*> FLWorldEnvironment::find_colliding_objects( FLWorldObject* object ) {
	std::vector<FLCollidingObject*> objects;
	std::vector<FLCollidingObject*>& colliding_objects = get_colliding_objects();

	for ( FLCollidingObject* other : colliding_objects ) {
		if ( other != nullptr && rect_collision( object, other ) )
			objects.push_back( other );
	}

	return objects;
}

void FLWorldEnvironment::mark_reset() {
	reset = true;
}

void FLWorldEnvironment::add_angel( NVAngel* angel ) {
	angels.push_back( angel );
}

