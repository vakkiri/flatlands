/*
 * 	net_object.cpp
 *
 */

#include <iostream>
#include <unordered_map>
#include "fl_net.h"
#include "net_object.h"

std::unordered_map<uint16_t, FLNetObject*> net_objects;


FLNetObject::FLNetObject() {
	static int last_net_id = 0;
	net_id = last_net_id + 1;
	last_net_id = net_id;
	add_net_obj( net_id, this );
}


FLNetObject::FLNetObject( uint16_t id ) {
	net_id = id;
}


FLNetObject::~FLNetObject() {
	destroy_net_obj( net_id, true );
	std::cout << "Net object deleted with id " << net_id << std::endl;
}


uint16_t FLNetObject::get_net_id() {
	return net_id;
}


void add_net_obj( uint16_t id, FLNetObject* obj ) {
	if ( net_objects.find(id) != net_objects.end() ) {
		std::cout << "ERROR: Duplicate net object id added.\n";
	}
	else {
		net_objects[id] = obj;
	}
}


void del_net_obj( uint16_t id ) {
	auto it = net_objects.find(id);

	while (  it != net_objects.end() ) {
		if ( it->second != nullptr ) {
			delete it->second;
		}

		net_objects.erase(it);
		it = net_objects.find(id);
	}
}


bool net_object_exists( uint16_t id ) {
	return ( net_objects.find(id) != net_objects.end() );
}

