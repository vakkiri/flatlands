/*
 * 	fl_collider.cpp
 *
 */

#include <iostream>

#include "fl_collider.h"
#include "../../environment/fl_environment.h"
#include "../../common/common.h"
#include "../../tilemap/tilemap.h"

FLCollider::FLCollider( FLGameObject* owner, std::string shape_name ) {
	if ( owner == nullptr ) {
		std::cout << "Error: cannot create collider with null owner.\n";
	}
	else if  ( owner->get_shape(shape_name) == nullptr ) {
		std::cout << "Error: cannot create collider with no shape.\n";
	}
	else {
		this->owner = owner;
		this->shape_name = shape_name;
	}
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
