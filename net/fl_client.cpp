/*
 * 	fl_client.cpp
 *
 */

#include <iostream>
#include "fl_client.h"

FLClient::FLClient() {
	initialized = false;
	socket = nullptr;

	server_conn.last_tick = 0;
	server_conn.state = FL_SERVER_DEAD;
}

void FLClient::start() {
	if ( SDLNet_ResolveHost( &my_ip, NULL, FL_CLIENT_PORT ) == -1 ) {
		std::cout << "Error resolving client host: " << SDLNet_GetError() << std::endl;
		return;
	}

	socket = SDLNet_UDP_Open(FL_CLIENT_PORT);

	if ( socket == nullptr ) {
		std::cout << "Unable to open port for client UDP socket.\n";
		return;
	}

	packet = SDLNet_AllocPacket(FL_MAX_PACKET_LEN);

	if ( packet == nullptr ) {
		std::cout << "Unable to create receiving UDP packet\n";
		return;
	}

	initialized = true;
	std::cout << "Client running on port " << FL_CLIENT_PORT << std::endl;
}

void FLClient::connect_to_server( std::string server_hostname ) {
	if ( SDLNet_ResolveHost( &server_ip, server_hostname.c_str(), FL_SERVER_PORT) == -1 ) {
		std::cout << "Client was uneable to resolve Server hostname.\n";
		return;
	}

	Uint8 data = 2;
	fl_send_udp( &data, 1, server_ip, socket );
}

void FLClient::update() {
	if ( initialized ) {
		check_conn();
		send();
		receive();
	}
}

void FLClient::send() {
	static int frame = 0;
	++frame;

	if ( frame % 60 == 0 ) {
		/* XXX TEST CODE */
		if ( server_conn.state != FL_SERVER_ALIVE ) {
			connect_to_server( "10.0.0.81" );
		}
		/* -- */
	}
}

void FLClient::receive() {
}

void FLClient::check_conn() {
}


