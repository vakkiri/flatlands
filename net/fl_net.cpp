/*
 * 	fl_net.cpp
 *
 */

#include <fstream>
#include <iostream>
#include "fl_net.h"
#include "fl_server.h"
#include "fl_client.h"

FLServer server;
FLClient client;

bool is_server;
bool is_client;

bool fl_init_net() {
	bool success = true;

	if ( SDLNet_Init() == -1 ) {
		std::cout << "Could not initialize SDL Net: " << SDLNet_GetError() << std::endl;
		success = false;
	}

	/* XXX Just test code */
	is_server = false;
	is_client = false;

	std::string conf_path = "test-assets/net.conf";
	std::ifstream conf_file;
	std::string line;
	std::string param;
	int val;

	conf_file.open(conf_path);
	if ( !conf_file.is_open() ) {
		std::cout << "Could not open network conf file.\n";
	}
	else {
		while ( !conf_file.eof() ) {
			getline( conf_file, line );
			if (line.size() > 0) {
				int pos = line.find(":");
				param = line.substr(0, pos);
				val = std::stoi(line.substr(pos+1, line.size() - pos));
				if ( param == "Server" && val == 1 ) {
					std::cout << "Network Server enabled.\n";
					is_server = true;
				}
				else if ( param == "Client" && val == 1 ) {
					std::cout << "Network Client enabled.\n";
					is_client = true;
				}
			}
		}
	}
	/* -- */

	return success;
}

void fl_start_server() {
	if ( is_server ) {
		server.start();
	}
	else {
		std::cout << "Ignoring call to start server: not enabled.\n";
	}
}

void fl_update_server() {
	server.update();
}

void fl_start_client() {
	if ( is_client ) {
		client.start();
	}
	else {
		std::cout << "Ignoring call to start client: not enabled.\n";
	}
}

void fl_update_client() {
	client.update();
}
