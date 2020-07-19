/*
 * 	fl_physics_handler.cpp
 *
 */

#include <algorithm>
#include <iostream>

#include "../../environment/fl_environment.h"
#include "../../tilemap/tilemap.h"
#include "../../world/physics_settings.h"
#include "../components.h"
#include "fl_physics_handler.h"

#define ON_GROUND_FRAMES 4
#define ON_WALL_FRAMES 2
#define WALL_SLIDE_FACTOR_UP 0.5
#define WALL_SLIDE_FACTOR_DOWN 0.05
#define DEFAULT_TERMINAL_VELOCITY_X (5.0f)
#define DEFAULT_TERMINAL_VELOCITY_Y (5.5f)

FLPhysicsHandler::FLPhysicsHandler() { _alive = false; }

bool FLPhysicsHandler::init(FLGameObject *owner, std::string collider_name) {
	bool success = true;
	if (owner == nullptr) {
		std::cout << "Error: Cannot create physics handler with no owner.\n";
		success = false;
	} else if (owner->get_collider(collider_name) == nullptr) {
		std::cout
			<< "Error: Invalid collider name passed to physics handler.\n";
		success = false;
	} else {
		this->owner = owner;
		this->collider_name = collider_name;
		vel.x = 0;
		vel.y = 0;
		gravity_factor = 1.0;
		friction_factor = 1.0;
		on_ground_timer = 0;
		on_wall_timer = 0;
		_alive = true;
	}

	return success;
}

void FLPhysicsHandler::update() {
	apply_gravity();
	apply_friction();
	bound_velocity();
	move();
	on_ground_timer -= 1;
	on_wall_timer -= 1;
}

void FLPhysicsHandler::apply_gravity() {
	float amt = FLPhysics::getInstance().gravity();

	if (on_wall()) {
		if (vel.y < 0) {
			amt *= WALL_SLIDE_FACTOR_UP;
		} else {
			amt *= WALL_SLIDE_FACTOR_DOWN;
		}
	}

	amt *= gravity_factor;

	vel.y += amt;
}

void FLPhysicsHandler::apply_friction() {
	float amt;

	if (on_ground()) {
		amt = FLPhysics::getInstance().friction();
	} else {
		amt = FLPhysics::getInstance().air_resist();
	}

	amt *= friction_factor;

	if (vel.x > 0) {
		vel.x = std::max(vel.x - amt, 0.f);
	} else if (vel.x < 0) {
		vel.x = std::min(vel.x + amt, 0.f);
	}
}

void FLPhysicsHandler::move() {
	FLCollider *collider = owner->get_collider(collider_name);
	bool top_touches_wall = false;;
	bool bot_touches_wall = false;;

	if (collider == nullptr) {
		owner->move(vel);
	} else {
		bool moved_x = false;
		bool moved_y = false;
		// move horizontally
		owner->move(vel.x, 0);
		
		// extra initial horizontal check for detecting wall slides
		// move vertically by the object's height and see if we
		// still have a horizontal collision
		owner->move(0, owner->h());

		if (	(vel.x > 0 && collider->right_touches_tilemap()) || 
			(vel.x < 0 && collider->left_touches_tilemap()) ) {
			bot_touches_wall = true;
		}

		// move back
		owner->move(0, -owner->h());

		if (vel.x > 0) {
			while (collider->right_touches_tilemap()) {
				top_touches_wall = true;
				owner->set_x(int(owner->x()) - 1);
				vel.x = 0;
				moved_x = true;
			}
			// anti-jitter
			if (moved_x) {
				owner->set_x(int(owner->x()) + 1);
			}
		} else if (vel.x < 0) {
			while (collider->left_touches_tilemap()) {
				top_touches_wall = true;
				owner->set_x(int(owner->x()) + 1);
				vel.x = 0;
				moved_x = true;
			}
		}
		
		// move vertically
		owner->move(0, vel.y);
		if (vel.y > 0) {
			while (collider->bottom_touches_tilemap()) {
				owner->set_y(int(owner->y()) - 1);
				on_ground_timer = ON_GROUND_FRAMES;
				vel.y = 0;
				moved_y = true;
			}
			// anti-jitter
			if (moved_y) {
				owner->set_y(int(owner->y()) + 1);
			}
		} else if (vel.y < 0) {
			while (collider->top_touches_tilemap()) {
				owner->set_y(int(owner->y()) + 1);
				vel.y = 0;
				moved_y = true;
			}
			// anti-jitter
			if (moved_y) {
				owner->set_y(int(owner->y()) - 1);
			}
		}
	}
	if (bot_touches_wall && top_touches_wall) {
		on_wall_timer = ON_WALL_FRAMES;
	}
}

bool FLPhysicsHandler::on_ground() { return on_ground_timer > 0; }

bool FLPhysicsHandler::on_wall() { return on_wall_timer > 0; }

void FLPhysicsHandler::accelerate(float x, float y) {
	vel.x += x;
	vel.y += y;
}

float FLPhysicsHandler::xvel() { return vel.x; }

float FLPhysicsHandler::yvel() { return vel.y; }

void FLPhysicsHandler::stop() {
	vel.x = 0;
	vel.y = 0;
}

void FLPhysicsHandler::stopx() { vel.x = 0; }

void FLPhysicsHandler::stopy() { vel.y = 0; }

bool FLPhysicsHandler::alive() { return _alive; }

void FLPhysicsHandler::kill() { _alive = false; }

void FLPhysicsHandler::set_gravity_factor(float factor) {
	this->gravity_factor = factor;
}

void FLPhysicsHandler::set_friction_factor(float factor) {
	this->friction_factor = factor;
}

void FLPhysicsHandler::bound_velocity() {
	if (vel.x > 0) {
		vel.x = std::min(vel.x, DEFAULT_TERMINAL_VELOCITY_X);
	} else if (vel.x < 0) {
		vel.x = std::max(vel.x, -DEFAULT_TERMINAL_VELOCITY_X);
	}
	if (vel.y > 0) {
		vel.y = std::min(vel.y, DEFAULT_TERMINAL_VELOCITY_Y);
	} else if (vel.y < 0) {
		vel.y = std::max(vel.y, -DEFAULT_TERMINAL_VELOCITY_Y);
	}
}
