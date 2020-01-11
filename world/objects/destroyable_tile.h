/*
 * 	destroyable_tile.h
 *
 * 	A tile which can be destroyed.
 *
 */

#ifndef DESTROYABLE_TILE_H_
#define DESTROYABLE_TILE_H_

#include "../../rendering/animated_object.h"

class FLDestroyableTile : public FLAnimatedObject {
	public:
		FLDestroyableTile( float x, float y );
};

#endif

