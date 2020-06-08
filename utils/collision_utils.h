/*
 * 	collision_utils.h
 *
 * 	Methods for detecting collisions.
 */

#ifndef COLLISION_UTILS_H_
#define COLLISION_UTILS_H_

#include "../common/basic_types.h"

class FLShape;

bool rect_collision(FLShape *a, FLShape *b);
bool point_in_shape(FLShape *shape, point *p);
point intersection(fl_line* l1, fl_line* l2);

#endif
