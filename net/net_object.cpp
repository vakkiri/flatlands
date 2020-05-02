/*
 * 	net_object.cpp
 *
 */

#include <iostream>
#include "net_object.h"

FLNetObject::FLNetObject() {
	static int last_net_id = 0;
	net_id = last_net_id + 1;
	last_net_id = net_id;
}

FLNetObject::FLNetObject( uint16_t id ) {
	net_id = id;
}

FLNetObject::~FLNetObject() {
	std::cout << "Net object deleted with id " << net_id << std::endl;
}

uint16_t FLNetObject::get_net_id() {
	return net_id;
}
