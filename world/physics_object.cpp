/*
 * 	physics_object.cpp
 *
 */

#include "physics_object.h"

FLPhysicsObject::FLPhysicsObject() : FLPhysicsObject( 0, 0, 0, 0 ) {};

FLPhysicsObject::FLPhysicsObject( float x, float y, float w, float h ) : FLWorldObject( x, y, w, h ) {
	bounds_margin.x = 0;
	bounds_margin.y = 0;
	bounds_margin.w = 0;
	bounds_margin.h = 0;
};

void FLPhysicsObject::set_bounds_margin( rect& new_bounds ) {
	bounds_margin.x = new_bounds.x;
	bounds_margin.y = new_bounds.y;
	bounds_margin.w = new_bounds.w;
	bounds_margin.h = new_bounds.h;
}

float FLPhysicsObject::bounds_x() { return position.x + bounds_margin.x; }
float FLPhysicsObject::bounds_y() { return position.y + bounds_margin.y; }
float FLPhysicsObject::bounds_w() { return position.w + bounds_margin.w; }
float FLPhysicsObject::bounds_h() { return position.h + bounds_margin.h; }

bool FLPhysicsObject::collides_with( FLPhysicsObject& other ) {
	// TODO: Implement rect-rect collision
	return false;
}

void FLPhysicsObject::update_physics() {
	vel += accel;
	move( vel );
}

void FLPhysicsObject::stop_horizontal() { 
	vel.x = 0;
	accel.x = 0;
}

void FLPhysicsObject::stop_vertical() { 
	vel.y = 0; 
	accel.y = 0;
}

void FLPhysicsObject::set_vel( point vel ) {
	this->vel = vel;
}

void FLPhysicsObject::accelerate( point amt ) {
	vel += amt;
}

void FLPhysicsObject::move( point amt ) {
	position.x += amt.x;
	position.y += amt.y;
}
