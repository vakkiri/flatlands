/*
 * 	destroyable_tile.h
 *
 * 	A tile which can be destroyed.
 *
 */

#ifndef DESTROYABLE_TILE_H_
#define DESTROYABLE_TILE_H_

#include "../../rendering/animated_object.h"
#include "../colliding_object.h"

class FLDestroyableTile : virtual public FLCollidingObject, virtual public FLAnimatedObject {
	public:
		FLDestroyableTile( float x, float y );
		virtual ~FLDestroyableTile();
		virtual void collide_with();
		virtual float bounds_h();
		virtual float bounds_y();
		virtual void update_animation();
};

#endif

