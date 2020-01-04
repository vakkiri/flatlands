/*
 * 	physics_object.cpp
 *
 */

#include "physics_object.h"
#include "physics_settings.h"
#include "world_environment.h"

#define PHYSICS_EPSILON 0.1

FLPhysicsObject::FLPhysicsObject() : FLPhysicsObject( 0, 0, 0, 0 ) {};

FLPhysicsObject::FLPhysicsObject( float x, float y, float w, float h ) : FLWorldObject( x, y, w, h ), physics(FLPhysics::getInstance()), environment(FLWorldEnvironment::getInstance()) {
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

void FLPhysicsObject::update_position() {
	// check for collisions with tilemap
	point next = point(bounds_x(), bounds_y()) + vel;

	// y direction:
	// moving down:
	if ( vel.y > 0 ) {
		if (environment.solid_at(bounds_x(), next.y + bounds_h())) {
			// move feet to tile top
			int tile_pos = int(next.y + bounds_h());
			tile_pos -= (tile_pos % 8);
			position.y = tile_pos - bounds_h() - PHYSICS_EPSILON;

			while (environment.solid_at(bounds_x(), bounds_y() + bounds_h())) {
				position.y -= 8;
			}
			stop_vertical();
		}
	}

	// moving up:
	else if ( vel.y < 0 ) {
		if (environment.solid_at(bounds_x(), next.y)) {
			// move head to tile bottom
			int tile_pos = int(next.y);
			tile_pos += (tile_pos % 8);
			position.y = tile_pos + PHYSICS_EPSILON;
				
			while (environment.solid_at(bounds_x(), bounds_y())) {
				position.y += 8;
			}
			stop_vertical();
		}
	}

	// x direction:
	// moving right:
	if ( vel.x > 0 ) {
		if (environment.solid_at(next.x + bounds_w(), bounds_y())) {
			int tile_pos = int(next.x + bounds_w());
			tile_pos += (tile_pos % 8);
			position.x = tile_pos - bounds_h() - PHYSICS_EPSILON;

			while (environment.solid_at(bounds_x() + bounds_w(), bounds_y())) {
				position.x -= 8;
			}
			stop_horizontal();
		}
	}
	// moving left:
	if ( vel.x > 0 ) {
		if (environment.solid_at(next.x, bounds_y())) {
			int tile_pos = int(next.y + bounds_h());
			tile_pos += (tile_pos % 8);
			position.x = tile_pos + PHYSICS_EPSILON;

			while (environment.solid_at(bounds_x(), bounds_y())) {
				position.x += 8;
			}
			stop_horizontal();
		}
	}

	move( vel );
}

void FLPhysicsObject::update_physics() {
	accel.y += physics.gravity();
	vel += accel;

	update_position();
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
