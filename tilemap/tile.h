/*
 * 	tile.h
 *
 * 	Simple tile class (a basic rendered quad).
 *
 */

#ifndef TILE_H_
#define TILE_H_

#include "../world/world_object.h"
#include "../rendering/textured_object.h"

class FLTile : public FLWorldObject, public FLTexturedObject {
	public:
		FLTile();
		FLTile( float x, float y, float w, float h );

		float x();
		float y();
		float w();
		float h();
};

#endif
