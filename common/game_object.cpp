/*
 * 	game_object.cpp
 *
 */

#include "game_object.h"

FLGameObject::FLGameObject( float x, float y, float w, float h ) {
	position.x = x;
	position.y = y;
	position.w = w;
	position.h = h;
}

void FLGameObject::set_x( float x ) { position.x = x; }
void FLGameObject::set_y( float y ) { position.y = y; }

float FLGameObject::x() { return position.x; }
float FLGameObject::y() { return position.y; }
float FLGameObject::w() { return position.w; }
float FLGameObject::h() { return position.h; }
