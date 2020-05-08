/*
 * 	net_object.cpp
 *
 */

#include <iostream>
#include <unordered_map>
#include <vector>
#include "fl_net.h"
#include "net_object.h"

std::unordered_map<uint16_t, FLNetObject*> net_objects;

/*
 *	When the server first finds out about a net object being deleted, it
 *	adds an entry with the object's ID to deleted_net_objects.
 *
 *	This associated vector is used to keep track of which clients have also
 *	deleted the net object. As soon as every element in the vector is "true", we
 *	know that every client has deleted the object and can remove the entry.
 *
 *	We intermittently will send messages to any clients who have not yet confirmed 
 *	the deletion of a net object.
 *
 */

std::unordered_map<uint16_t, std::vector<bool>> deleted_net_objects;

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
	del_net_obj( net_id );
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
	if ( it != net_objects.end() ) {
		it->second->on_delete_msg();
		net_objects.erase(it);
	}
}

bool net_object_exists( uint16_t id ) {
	return ( net_objects.find(id) != net_objects.end() );
}

void mark_net_object_deleted( uint16_t id, int client_slot ) {
	// If client_slot is -1 then the server made this request
	if ( net_object_exists(id) || client_slot == -1 ) {
		deleted_net_objects[id] = std::vector<bool>(FL_MAX_CONN, false);

		if (client_slot >= 0)
			deleted_net_objects[id][client_slot] = true;
	}
	// Else, we received a message from a client about an item that does not exist for anyone else...
}

