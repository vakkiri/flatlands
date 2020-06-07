/*
 * 	collision_utils.h
 *
 * 	Methods for detecting collisions.
 */

#ifndef COLLISION_UTILS_H_
#define COLLISION_UTILS_H_

class FLShape;
struct point;

bool rect_collision(FLShape *a, FLShape *b);
bool point_in_shape(FLShape *shape, point *p);

#endif
