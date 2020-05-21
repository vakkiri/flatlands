/*
 * 	fl_collider.cpp
 *
 */

#include <iostream>

#include "fl_collider.h"
#include "../../environment/fl_environment.h"
#include "../../common/common.h"
#include "../../tilemap/tilemap.h"

FLCollider::FLCollider() {
	_alive = false;
}

bool FLCollider::init( FLGameObject* owner, std::string shape_name, std::string name ) {
	bool success = true; 
	if ( owner == nullptr ) {
		std::cout << "Error: cannot create collider with null owner.\n";
		success = false;
	}
	else if  ( owner->get_shape(shape_name) == nullptr ) {
		std::cout << "Error: cannot create collider with no shape.\n";
		success = false;
	}
	else {
		this->owner = owner;
		this->shape_name = shape_name;
		_alive = true;
	}
	return success;
}

bool FLCollider::touches_tilemap() {
	FLShape* shape = owner->get_shape( shape_name );
	bool collision = false;

	if ( shape == nullptr ) {
		std::cout << "Error: checking collision with null shape.\n";
	}
	else {
		collision = owner->environment()->tilemap()->touches_shape( shape );
	}

	return collision;
}

bool FLCollider::bottom_touches_tilemap() {
	FLShape* shape = owner->get_shape( shape_name );
	bool collision = false;

	if ( shape == nullptr ) {
		std::cout << "Error: checking collision with null shape.\n";
	}
	else {
		point p1 = point( shape->x(), shape->y() + shape->h() );
		point p2 = point( shape->x() + shape->w(), shape->y() + shape->h() );
		collision = owner->environment()->tilemap()->touches_line( p1, p2 );
	}

	return collision;

}

bool FLCollider::top_touches_tilemap() {
	FLShape* shape = owner->get_shape( shape_name );
	bool collision = false;

	if ( shape == nullptr ) {
		std::cout << "Error: checking collision with null shape.\n";
	}
	else {
		point p1 = point( shape->x(), shape->y() );
		point p2 = point( shape->x() + shape->w(), shape->y() );
		collision = owner->environment()->tilemap()->touches_line( p1, p2 );
	}

	return collision;

}

bool FLCollider::left_touches_tilemap() {
	FLShape* shape = owner->get_shape( shape_name );
	bool collision = false;

	if ( shape == nullptr ) {
		std::cout << "Error: checking collision with null shape.\n";
	}
	else {
		point p1 = point( shape->x(), shape->y() );
		point p2 = point( shape->x(), shape->y() + shape->h() );
		collision = owner->environment()->tilemap()->touches_line( p1, p2 );
	}

	return collision;

}

bool FLCollider::right_touches_tilemap() {
	FLShape* shape = owner->get_shape( shape_name );
	bool collision = false;

	if ( shape == nullptr ) {
		std::cout << "Error: checking collision with null shape.\n";
	}
	else {
		point p1 = point( shape->x() + shape->w(), shape->y() );
		point p2 = point( shape->x() + shape->w(), shape->y() + shape->h() );
		collision = owner->environment()->tilemap()->touches_line( p1, p2 );
	}

	return collision;

}

void FLCollider::add_collision_group( std::string group ) {
	collision_groups.insert( group );
}

void FLCollider::add_target_collision_group( std::string group ) {
	target_collision_groups.insert( group );
}

std::unordered_set<std::string>& FLCollider::get_collision_groups() {
	return collision_groups;
}

bool FLCollider::alive() {
	return _alive;
}

void FLCollider::kill() {
	_alive = false;
}

void FLCollider::update() {

}

