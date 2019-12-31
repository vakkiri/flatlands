/*
 * 	world_object.cpp
 *
 */

#include "world_object.h"

FLWorldObject::FLWorldObject( float x, float y, float w, float h ) {
	position.x = x;
	position.y = y;
	position.w = w;
	position.h = h;
}

float FLWorldObject::x() { return position.x; }
float FLWorldObject::y() { return position.y; }
float FLWorldObject::w() { return position.w; }
float FLWorldObject::h() { return position.h; }
float FLWorldObject::bounds_x() { return x(); }
float FLWorldObject::bounds_y() { return y(); }
float FLWorldObject::bounds_w() { return w(); }
float FLWorldObject::bounds_h() { return h(); }

