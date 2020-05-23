/*
 * 	fl_environment.cpp
 *
 */

#include <iostream>
#include "fl_environment.h"

#include "../world/player/player.h"
#include "../resources/fl_resources.h"
#include "../tilemap/tilemap.h"

FLEnvironment::FLEnvironment() {
	_player = new FLPlayer();
	_tilemap = new FLTilemap(1024, 1024, 16);
}

FLEnvironment::~FLEnvironment() {
	delete _player;
	delete _tilemap;

	while ( !objects.empty() ) {
		delete objects.back();
		objects.pop_back();
	}
}

void FLEnvironment::load_level( int id ) {
	FLResources::getInstance().load_level( id, this );
}

void FLEnvironment::add_object( FLGameObject* object ) {
	objects.push_back( object );
}

FLPlayer* FLEnvironment::player() {
	return _player;
}

FLTilemap* FLEnvironment::tilemap() {
	return _tilemap;
}

