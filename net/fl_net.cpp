/*
 * 	fl_net.cpp
 *
 */

#include <iostream>
#include "fl_net.h"
#include "fl_server.h"
#include "fl_client.h"

FLServer server;
FLClient client;

bool fl_init_net() {
	bool success = true;
	if ( SDLNet_Init() == -1 ) {
		std::cout << "Could not initialize SDL Net: " << SDLNet_GetError() << std::endl;
		success = false;
	}
	return success;
}

void fl_start_server() {
	server.start();
}

void fl_update_server() {
	server.update();
}

void fl_start_client() {
	client.start();
}

void fl_update_client() {
	client.update();
}
