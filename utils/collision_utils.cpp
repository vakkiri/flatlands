/*
 * 	collision_utils.cpp
 *
 */


#include "collision_utils.h"
#include "../logging/logging.h"
#include "../world/world_object.h"

bool rect_collision( FLWorldObject* a, FLWorldObject* b ) {
	if ( a == nullptr ) {
		log_error( "First paramter of rect_collision is null" );
		return false;
	}
	if ( b == nullptr ) {
		log_error( "Second paramter of rect_collision is null" );
		return false;
	}

	float ax = a->bounds_x();
	float ay = a->bounds_y();
	float aw = a->bounds_w();
	float ah = a->bounds_h();

	float bx = b->bounds_x();
	float by = b->bounds_y();
	float bw = b->bounds_w();
	float bh = b->bounds_h();

	if ( ax > bx + bw || bx > ax + aw )
		return false;

	if ( ay > by + bh || by > ay + ah )
		return false;

	return true;
}

