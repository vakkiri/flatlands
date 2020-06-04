/*
 * 	game_object.cpp
 *
 */

#include <iostream>
#include <utility>

#include "../components/components.h"
#include "../game/fl_game.h"
#include "../rendering/animated_object.h"
#include "fl_shape.h"
#include "game_object.h"

// TODO: should this be removed, or should I just initialize a large list of
// game objects at start?
FLGameObject::FLGameObject() : FLGameObject(0, 0, 0, 0) {}

FLGameObject::FLGameObject(float x, float y, float w, float h) {
	FLShape *position = new FLShape(x, y, w, h);
	shapes.insert(std::make_pair("position", position));

	physics_handler_handle = -1;
	updator_handle = -1;

	parent = nullptr;
}

FLGameObject::~FLGameObject() {
	for (auto kv : shapes) {
		delete kv.second;
	}
	for (auto kv : colliders) {
		fl_delete_collider(kv.second);
	}
	for (auto kv : animators) {
		delete kv.second;
	}

	delete_physics_handler(physics_handler_handle);
	delete_updator(updator_handle);
}

void FLGameObject::set_x(float x) {
	// We use dx instead of just setting all positions to x, so that we can keep
	// relative offsets between the object's position and all other shapes.
	float dx = x - shapes["position"]->x();

	for (auto kv : shapes) {
		kv.second->translate(dx, 0);
	}
}

void FLGameObject::set_y(float y) {
	// We use dy instead of just setting all positions to y, so that we can keep
	// relative offsets between the object's position and all other shapes.
	float dy = y - shapes["position"]->y();

	for (auto kv : shapes) {
		kv.second->translate(0, dy);
	}
}

float FLGameObject::x() {
	float x = shapes["position"]->x();
	if (parent != nullptr) {
		x += parent->x();
	}
	return x;
}

float FLGameObject::y() {
	float y = shapes["position"]->y();
	if (parent != nullptr) {
		y += parent->y();
	}
	return y;
}

float FLGameObject::w() { return shapes["position"]->w(); }
float FLGameObject::h() { return shapes["position"]->h(); }

void FLGameObject::move(float x, float y) {
	for (auto kv : shapes) {
		kv.second->translate(x, y);
	}
}

void FLGameObject::move(point amt) {
	for (auto kv : shapes) {
		kv.second->translate(amt);
	}
}

void FLGameObject::movex(float x) {
	for (auto kv : shapes) {
		kv.second->translate(x, 0);
	}
}

void FLGameObject::movey(float y) {
	for (auto kv : shapes) {
		kv.second->translate(0, y);
	}
}

FLShape *FLGameObject::get_shape(std::string name) {
	if (shapes.find(name) == shapes.end()) {
		return nullptr;
	} else {
		return shapes[name];
	}
}

FLCollider *FLGameObject::get_collider(std::string name) {
	if (colliders.find(name) == colliders.end()) {
		return nullptr;
	} else {
		return fl_get_collider(colliders[name]);
	}
}

void FLGameObject::add_collider(std::string shape, std::string name) {
	int result = -1;
	if (shapes.find(shape) != shapes.end()) {
		result = fl_new_collider(this, shape);

		if (result >= 0) {
			colliders.insert(std::make_pair(name, result));
		} else {
			std::cout << "Error: could not create new collider.\n";
		}
	} else {
		std::cout << "Error: invalid shape provided to collider.\n";
	}
}

FLPhysicsHandler *FLGameObject::physics_handler() {
	return (get_physics_handler(physics_handler_handle));
}

FLEnvironment *FLGameObject::environment() {
	return FLGame::instance().environment();
}

void FLGameObject::set_parent(FLGameObject *obj) { parent = obj; }

FLAnimatedObject *FLGameObject::get_animator(std::string name) {
	FLAnimatedObject *anim = nullptr;

	auto it = animators.find(name);
	if (it != animators.end()) {
		anim = it->second;
	}

	return anim;
}
