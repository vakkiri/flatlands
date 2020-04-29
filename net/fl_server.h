/*
 * 	fl_server.h
 *
 * 	Server class for flatlands.
 *
 */

#ifndef FL_NET_SERVER_H_
#define FL_NET_SERVER_H_

#include <queue>
#include "fl_net.h"

enum FLClientState {
	FL_CLIENT_CONNECTED,
	FL_CLIENT_DISCONNECTED,
	FL_CLIENT_RECONNECTING
};

struct FLClientConn {
	bool accepted;
	IPaddress ip;
	int ping;
	Uint32 last_tick;
	Uint32 last_heartbeat;
	FLClientState state;
};

class FLServer {
	public:
		FLServer();
		void start();
		void update();

	protected:
		void queue_message(int slot, Uint8* data, int len);

		void queue_heartbeats();
		void queue_heartbeat(int slot);

		void check_conns();
		void receive();
		void send();
		void handle_packet();
		void accept_client_conn(IPaddress addr);
		void accept_heartbeat(IPaddress addr);
		void reconnect_client(int slot);


		bool initialized;

		std::queue<FLNetMessage*> udp_message_queue;

		UDPpacket* packet;	// used for all received packets

		IPaddress my_ip;
		FLClientConn client_conns[FL_MAX_CONN];
		UDPsocket socket;
};

#endif

