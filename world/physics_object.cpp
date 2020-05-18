/*
 * 	physics_object.cpp
 *
 */

#include <algorithm>

#include "../components/components.h"

#include "physics_object.h"
#include "physics_settings.h"
#include "world_environment.h"

#define PHYSICS_EPSILON 0.01
#define ON_GROUND_GRACE_FRAMES 2

FLPhysicsObject::FLPhysicsObject() : FLPhysicsObject( 0, 0, 0, 0 ) {};

FLPhysicsObject::FLPhysicsObject( float x, float y, float w, float h ) : FLWorldObject( x, y, w, h ), physics(FLPhysics::getInstance()), environment(FLWorldEnvironment::getInstance()) {
	bounds_margin.x = 0;
	bounds_margin.y = 0;
	bounds_margin.w = 0;
	bounds_margin.h = 0;

	on_ground_timer = 0;
};

void FLPhysicsObject::set_bounds_margin( rect new_bounds ) {
	bounds_margin.x = new_bounds.x;
	bounds_margin.y = new_bounds.y;
	bounds_margin.w = new_bounds.w;
	bounds_margin.h = new_bounds.h;
}

float FLPhysicsObject::bounds_x() { return x() + bounds_margin.x; }
float FLPhysicsObject::bounds_y() { return y() + bounds_margin.y; }
float FLPhysicsObject::bounds_w() { return w() + bounds_margin.w; }
float FLPhysicsObject::bounds_h() { return h() + bounds_margin.h; }

bool FLPhysicsObject::collides_with( FLPhysicsObject& other ) {
	// TODO: Implement rect-rect collision
	return false;
}

void FLPhysicsObject::update_position() {
	if ( colliders.find("tilemap") == colliders.end() ) {
		move( vel );
	}
	else {
		FLCollider* collider = colliders["tilemap"];

		// horizontal movement
		move( vel.x, 0 );
		if ( vel.x > 0 ) {
			if ( collider->right_touches_tilemap() ) {
				set_x( int(x() - ( int(x()) % 8 )) );
				stop_horizontal();
			}
		}
		else if ( vel.x < 0 ) {
			if ( collider->left_touches_tilemap() ) {
				set_x( int(x() + ( 8 - (int(x()) % 8) )) );
				stop_horizontal();
			}
		}

		// vertical movement
		move( 0, vel.y );
		if ( vel.y > 0 ) {
			if ( collider->bottom_touches_tilemap() ) {
				set_y( int(y() - ( int(y()) % 8 )) );
				set_on_ground();
				stop_vertical();
			}
		}
		else if ( vel.y < 0 ) {
			if ( collider->top_touches_tilemap() ) {
				set_y( int(y() + ( 8 - (int(y()) % 8) )) );
				move( 0, 1 );
				stop_vertical();
			}
		}
	}
}

void FLPhysicsObject::apply_gravity() {
	accel.y += physics.gravity();
}

void FLPhysicsObject::apply_friction() {
	float amt;

	if ( on_ground() )
		amt = physics.friction();
	else
		amt = physics.air_resist();

	if (vel.x > 0)
		vel.x = std::max(vel.x - amt, 0.f);
	else if (vel.x < 0)
		vel.x = std::min(vel.x + amt, 0.f);
}

void FLPhysicsObject::update_physics() {
	if ( on_ground_timer > 0 )
		on_ground_timer--;

	apply_gravity();
	apply_friction();

	vel += accel;
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

bool FLPhysicsObject::on_ground() {
	return on_ground_timer > 0;
}

void FLPhysicsObject::set_on_ground() {
	on_ground_timer = ON_GROUND_GRACE_FRAMES;
	stop_vertical();
}

point& FLPhysicsObject::get_vel() {
	return vel;
}

