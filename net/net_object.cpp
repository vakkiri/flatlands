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
	// Remove this item from our list
	if ( net_objects.find(net_id) != net_objects.end() ) {
		net_objects.erase(net_id);
	}
	// Synchronize deletion over the net
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
	FLNetObject* obj;
	if ( net_objects.find(id) != net_objects.end() ) {
		std::cout << "found net obj to delete with id " << id << std::endl;
		obj = net_objects[id];
		net_objects.erase(id);
		delete obj;
	}
}


bool net_object_exists( uint16_t id ) {
	return ( net_objects.find(id) != net_objects.end() );
}

