/*
 * 	fl_collider.cpp
 *
 */

#include <iostream>

#include "fl_collider.h"
#include "fl_collider_manager.h"
#include "../../environment/fl_environment.h"
#include "../../common/common.h"
#include "../../tilemap/tilemap.h"
#include "../../utils/collision_utils.h"

FLCollider::FLCollider() {
	_alive = false;
	has_collision_method = false;
}

bool FLCollider::init( FLGameObject* owner, std::string shape_name ) {
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

FLShape* FLCollider::get_shape() {
	return owner->get_shape( shape_name );
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
	return _alive; }

void FLCollider::kill() {
	_alive = false;
}

void FLCollider::update() {
	detect_collisions();
	process_collisions();
}

void FLCollider::detect_collisions() {
	FLCollider* target;
	FLShape* target_shape;
	FLShape* shape = get_shape();

	for ( std::string group : target_collision_groups ) {
		for ( int object_handle : fl_get_group_colliders(group) ) {
			target = fl_get_collider( object_handle );

			if ( target != nullptr && (target_shape = target->get_shape()) != nullptr ) {
				if ( rect_collision( shape, target_shape ) ){
					collisions.push_back(target);
				}
			}
			else {
				std::cout << "Warning: a null collider is alive and being referenced.\n";
			}
		}
	}
}

void FLCollider::process_collisions() {
	if ( has_collision_method ) {
		while ( !collisions.empty() ) {
			FLCollider* collision = collisions.back();
			on_collision( collision );
			collisions.pop_back();
		}
	}
	else {
		collisions.clear();
	}
}

void FLCollider::set_collision_method( std::function<void(FLCollider*)> meth ) {
	on_collision = meth;
	has_collision_method = true;
}

void FLCollider::add_collision( FLCollider* collision ) {
	collisions.push_back( collision );
}

