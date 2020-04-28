/*
 * 	fl_client.h
 *
 * 	Client class for flatlands.
 *
 */

#ifndef FL_NET_CLIENT_H_
#define FL_NET_CLIENT_H_

#include "fl_net.h"

enum FLServerState {
	FL_SERVER_ALIVE,
	FL_SERVER_DEAD,
	FL_SERVER_SELF
};

struct FLServerConn {
	IPaddress ip;
	Uint32 last_tick;
	FLServerState state;
};

class FLClient {
	public:
		FLClient();
		void start();
		void update();

	protected:
		void connect_to_server( std::string server_hostname );
		void check_conn();
		void receive();
		void send();
		void handle_packet();
		
		bool initialized;

		UDPpacket* packet;		// used for all received packets
		FLServerConn server_conn;
		UDPsocket socket;
		IPaddress server_ip;
		IPaddress my_ip;
};

#endif

