/*
 * 	fl_server.h
 *
 * 	Server class for flatlands.
 *
 */

#ifndef FL_NET_SERVER_H_
#define FL_NET_SERVER_H_

#include "fl_net.h"

enum FLClientState {
	FL_CLIENT_CONNECTED,
	FL_CLIENT_DISCONNECTED,
	FL_CLIENT_RECONNECTING
};

struct FLClientConn {
	bool accepted;
	IPaddress ip;
	Uint32 last_tick;
	FLClientState state;
};

class FLServer {
	public:
		FLServer();
		void start();
		void update();

	protected:
		void check_conns();
		void receive();
		void send();
		void handle_packet();
		void accept_client_conn(IPaddress addr);
		void reconnect_client(int slot);

		bool initialized;

		//std::vector<UDPpack*> out_queue; idk, this should actually be FLMessage* or somethin

		UDPpacket* packet;	// used for all received packets

		IPaddress my_ip;
		FLClientConn client_conns[FL_MAX_CONN];
		UDPsocket socket;
};

#endif

