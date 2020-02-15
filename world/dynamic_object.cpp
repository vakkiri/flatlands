/*
 * 	dynamic_object.cpp
 *
 */

#include <iostream>
#include "dynamic_object.h"

std::vector<FLDynamicObject*> dynamic_objects;

FLDynamicObject::FLDynamicObject() {
	zombie = false;
	dynamic_list_position = dynamic_objects.size();
	dynamic_objects.push_back( this );
}

FLDynamicObject::~FLDynamicObject() {
	// Set the corresponding entry in dynamic_objects to nullptr
	dynamic_objects[dynamic_list_position] = nullptr;
}

bool FLDynamicObject::is_zombie() {
	return zombie;
}

void FLDynamicObject::set_dynamic_list_position( unsigned int position ) {
	this->dynamic_list_position = position;
}

void update_dynamic_objects() {
	unsigned int null_elements = 0;

	// First, clear any null objects.
	// To optimize for bullet hell scenarios, we allow for null elements in this list
	// and clear them in one pass as we find them.

	// We subtract null elements from the size of the list to avoid iterating over
	// already cleared elements.
	for ( unsigned int i = 0; i < dynamic_objects.size() - null_elements; ++i ) {

		// If the object is a zombie, delete and null it
		if ( dynamic_objects[i] != nullptr && dynamic_objects[i]->is_zombie() )
			delete dynamic_objects[i];

		if ( dynamic_objects[i] != nullptr )
			dynamic_objects[i]->update();
		else {
			// When we find a null object, we swap it with the last non-null
			// object. 
			// Here we first skip all null elements which happened to be at
			// the back of the list.
			while ( (null_elements + i < dynamic_objects.size()) && dynamic_objects[dynamic_objects.size() - 1 - null_elements]  == nullptr )
				null_elements++;

			if ( null_elements < dynamic_objects.size() ) {
				dynamic_objects[i] = dynamic_objects[dynamic_objects.size() - 1 - null_elements];
				dynamic_objects[i]->set_dynamic_list_position(i);
				dynamic_objects[dynamic_objects.size() - 1 - null_elements] = nullptr;
				null_elements++;
			}
		}
	}

	// Now clean up all the null elements at the back
	while ( !dynamic_objects.empty() && dynamic_objects.back() == nullptr )
		dynamic_objects.pop_back();

}

void clear_dynamic_objects() {
	while ( !dynamic_objects.empty() ) {
		if ( dynamic_objects.back() != nullptr )
			delete dynamic_objects.back();
		dynamic_objects.pop_back();
	}
}

