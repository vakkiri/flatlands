/*
 * 	world_environment.cpp
 *
 */

#include "world_environment.h"
#include "../tilemap/tilemap.h"

FLTilemap* FLWorldEnvironment::tilemap() { return _tilemap; }

void FLWorldEnvironment::set_tilemap(FLTilemap* tilemap) {
	_tilemap = tilemap;
}

bool FLWorldEnvironment::solid_at( float x, float y ) {
	return _tilemap->solid_at( x, y );
}

