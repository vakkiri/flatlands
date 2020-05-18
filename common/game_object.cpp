/*
 * 	game_object.cpp
 *
 */

#include <iostream>
#include <utility>

#include "game_object.h"
#include "fl_shape.h"
#include "../components/components.h"

// TODO: Once component based refactor is done I should be able to remove the null constructor
FLGameObject::FLGameObject() : FLGameObject( 0, 0, 0, 0 ) {}

FLGameObject::FLGameObject( float x, float y, float w, float h ) {
	FLShape* position = new FLShape( x, y, w, h );
	shapes.insert( std::make_pair("position", position) );
}

FLGameObject::~FLGameObject() {
	for ( auto kv : shapes ) {
		delete kv.second;
	}
	for ( auto kv : colliders ) {
		delete kv.second;
	}
}

void FLGameObject::set_x( float x ) { 
	shapes["position"]->set_pos( x, y() ); 
}

void FLGameObject::set_y( float y ) { 
	shapes["position"]->set_pos( x(), y ); 
}

float FLGameObject::x() { return shapes["position"]->x(); }
float FLGameObject::y() { return shapes["position"]->y(); }
float FLGameObject::w() { return shapes["position"]->w(); }
float FLGameObject::h() { return shapes["position"]->h(); }

void FLGameObject::move( float x, float y ) {
	shapes["position"]->translate( x, y );
}

void FLGameObject::move( point amt ) {
	shapes["position"]->translate( amt );
}

void FLGameObject::movex( float x ) {
	shapes["position"]->translate( x, 0 );
}

void FLGameObject::movey( float y ) {
	shapes["position"]->translate( 0, y );
}

FLShape* FLGameObject::get_shape( std::string name ) {
	if ( shapes.find(name) == shapes.end() ) {
		return nullptr;
	}
	else {
		return shapes[name];
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

