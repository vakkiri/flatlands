/*
 * 	colliding_object.cpp
 *
 */

#include "colliding_object.h"

std::vector<FLCollidingObject*> colliding_objects;

FLCollidingObject::FLCollidingObject() {
	colliding_object_list_pos = colliding_objects.size();
	colliding_objects.push_back( this );
}

FLCollidingObject::~FLCollidingObject() {
	colliding_objects[colliding_object_list_pos] = nullptr;
}

void clear_colliding_objects() {
	while ( !colliding_objects.empty() ) {
		if ( colliding_objects.back() != nullptr )
			delete colliding_objects.back();
		colliding_objects.pop_back();
	}
}

std::vector<FLCollidingObject*>& get_colliding_objects() {
	return colliding_objects;
}

