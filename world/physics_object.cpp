/*
 * 	physics_object.cpp
 *
 */

#include <algorithm>

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

float FLPhysicsObject::bounds_x() { return position.x + bounds_margin.x; }
float FLPhysicsObject::bounds_y() { return position.y + bounds_margin.y; }
float FLPhysicsObject::bounds_w() { return position.w + bounds_margin.w; }
float FLPhysicsObject::bounds_h() { return position.h + bounds_margin.h; }

bool FLPhysicsObject::collides_with( FLPhysicsObject& other ) {
	// TODO: Implement rect-rect collision
	return false;
}

void FLPhysicsObject::update_position() {
	// check for collisions with tilemap
	point next = point(bounds_x(), bounds_y()) + vel;

	// y direction:
	// moving down:
	if ( vel.y > 0 ) {
		// check bottom left + bottom right
		if (environment.solid_at(bounds_x(), next.y + bounds_h()) ||
		    environment.solid_at(x() + bounds_w(), next.y + bounds_h())) {
			// move feet to tile top
			int tile_pos = int(next.y + bounds_h());
			tile_pos -= (tile_pos % 8);
			position.y = tile_pos - bounds_h() - bounds_margin.y - PHYSICS_EPSILON;

			on_ground_timer = ON_GROUND_GRACE_FRAMES;
			stop_vertical();
		}
	}

	// moving up:
	else if ( vel.y < 0 ) {
		// check top left and top right
		if (environment.solid_at(bounds_x(), next.y) ||
		    environment.solid_at(x() + bounds_w(), next.y)) {
			// move head to tile bottom
			int tile_pos = int(next.y);
			int diff = 8 - (tile_pos % 8);
			position.y = tile_pos + diff + PHYSICS_EPSILON;
				
			stop_vertical();
		}
	}

	// x direction:
	// moving right:
	if ( vel.x > 0 ) {
		// check top right and bottom right
		if (environment.solid_at(next.x + bounds_w(), bounds_y()) ||
		    environment.solid_at(next.x + bounds_w(), bounds_y() + bounds_h())) {
			int tile_pos = int(next.x + bounds_w());
			int diff = tile_pos % 8;
			position.x = tile_pos - diff - bounds_w() - PHYSICS_EPSILON;

			stop_horizontal();
		}
	}
	// moving left:
	else if ( vel.x < 0 ) {
		// check top left and bottom left
		if (environment.solid_at(next.x, bounds_y()) ||
		    environment.solid_at(next.x, bounds_y() + bounds_h())) {
			int tile_pos = int(next.x);
			int diff = 8 - (tile_pos % 8);
			position.x = tile_pos + diff + PHYSICS_EPSILON;

			stop_horizontal();
		}
	}

	move( vel );
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

void FLPhysicsObject::move( point amt ) {
	position.x += amt.x;
	position.y += amt.y;
}

bool FLPhysicsObject::on_ground() {
	return on_ground_timer > 0;
}

