/*
 * 	collision_utils.cpp
 *
 */

#include "collision_utils.h"
#include "../common/fl_shape.h"

bool rect_collision(FLShape *a, FLShape *b) {
	if (a == nullptr) {
		return false;
	}
	if (b == nullptr) {
		return false;
	}

	float ax = a->x();
	float ay = a->y();
	float aw = a->w();
	float ah = a->h();

	float bx = b->x();
	float by = b->y();
	float bw = b->w();
	float bh = b->h();

	if (ax > bx + bw || bx > ax + aw)
		return false;

	if (ay > by + bh || by > ay + ah)
		return false;

	return true;
}
