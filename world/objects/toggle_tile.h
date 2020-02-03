/*
 * 	toggle_tile.h
 *
 * 	A tile which can toggle between a solid and passable state.
 *
 */

#ifndef TOGGLE_TILE_H_
#define TOGGLE_TILE_H_

#include <vector>
#include "../../rendering/animated_object.h"
#include "../colliding_object.h"

class FLToggleTile : virtual public FLAnimatedObject {
	public:
		FLToggleTile( float x, float y, bool on);
		virtual ~FLToggleTile();
		virtual void update_animation();
		virtual void toggle();
	protected:
		bool on;
};

std::vector<FLToggleTile*>& get_toggle_tiles();
void clear_toggle_tiles();

#endif

