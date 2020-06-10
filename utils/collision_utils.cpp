/*
 * 	collision_utils.cpp
 *
 */

#include <iostream>
#include "collision_utils.h"
#include "../common/fl_shape.h"
#include "../common/basic_types.h"

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

bool point_in_shape(FLShape* shape, point* p) {
	return (	p->x >= shape->x() && 
			p->x <= shape->x() + shape->w() &&
			p->y >= shape->y() &&
			p->y <= shape->y() + shape->h() );
}

point intersection(fl_line* l1, fl_line* l2) {
	float s, t, d;
	point p = {-1, -1};

	d = (l2->v.x * l1->v.y) - (l1->v.x * l2->v.y);

	if (d != 0.f) {
		s = (1 / d) * ((l1->u.x - l2->u.x) * l1->v.y - (l1->u.y - l2->u.y) * l1->v.x);
		t = (1 / d) * (-((-(l1->u.x - l2->u.x)) * l2->v.y + (l1->u.y - l2->u.y) * l2->v.x));

		if (s >= 0 && t >= 0 && s <= 1 && t <= 1) {
			p.x = l1->u.x + s * l1->v.x;
			p.y = l1->u.y + t * l1->v.y;
		}
	}

	return p;
}

