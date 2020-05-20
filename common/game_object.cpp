/*
 * 	game_object.cpp
 *
 */

#include <iostream>
#include <utility>

#include "game_object.h"
#include "fl_shape.h"
#include "../components/components.h"
#include "../game/fl_game.h"

// TODO: Once component based refactor is done I should be able to remove the null constructor
FLGameObject::FLGameObject() : FLGameObject( 0, 0, 0, 0 ) {}

FLGameObject::FLGameObject( float x, float y, float w, float h ) {
	FLShape* position = new FLShape( x, y, w, h );
	shapes.insert( std::make_pair("position", position) );

	physics_handler_handle = -1;
	updator_handle = -1;
}

FLGameObject::~FLGameObject() {
	for ( auto kv : shapes ) {
		delete kv.second;
	}
	for ( auto kv : colliders ) {
		delete kv.second;
	}

	delete_physics_handler( physics_handler_handle );
	delete_updator( updator_handle );
}

void FLGameObject::set_x( float x ) { 
	// We use dx instead of just setting all positions to x, so that we can keep
	// relative offsets between the object's position and all other shapes.
	float dx = x - shapes["position"]->x();

	for ( auto kv : shapes ) {
		kv.second->translate( dx, 0 );
	}
}

void FLGameObject::set_y( float y ) { 
	// We use dy instead of just setting all positions to y, so that we can keep
	// relative offsets between the object's position and all other shapes.
	float dy = y - shapes["position"]->y();

	for ( auto kv : shapes ) {
		kv.second->translate( 0, dy );
	}
}

float FLGameObject::x() { return shapes["position"]->x(); }
float FLGameObject::y() { return shapes["position"]->y(); }
float FLGameObject::w() { return shapes["position"]->w(); }
float FLGameObject::h() { return shapes["position"]->h(); }

void FLGameObject::move( float x, float y ) {
	for ( auto kv : shapes ) {
		kv.second->translate( x, y );
	}
}

void FLGameObject::move( point amt ) {
	for ( auto kv : shapes ) {
		kv.second->translate( amt );
	}
}

void FLGameObject::movex( float x ) {
	for ( auto kv : shapes ) {
		kv.second->translate( x, 0 );
	}
}

void FLGameObject::movey( float y ) {
	for ( auto kv : shapes ) {
		kv.second->translate( 0, y );
	}
}

FLShape* FLGameObject::get_shape( std::string name ) {
	if ( shapes.find(name) == shapes.end() ) {
		return nullptr;
	}
	else {
		return shapes[name];
	}
}

FLCollider* FLGameObject::get_collider( std::string name ) {
	if ( colliders.find(name) == colliders.end() ) {
		return nullptr;
	}
	else {
		return colliders[name];
	}
}

void FLGameObject::add_collider( std::string shape, std::string name ) {
	if ( shapes.find(shape) != shapes.end() ) {
		FLCollider* collider = new FLCollider( this, shape );
		colliders.insert( std::make_pair(name, collider) );
	}
	else {
		std::cout << "Error: invalid shape provided to collider.\n";
	}
}

FLPhysicsHandler* FLGameObject::physics_handler() {
	return ( get_physics_handler(physics_handler_handle) );
}

FLEnvironment* FLGameObject::environment() {
	return FLGame::instance().environment();
}
