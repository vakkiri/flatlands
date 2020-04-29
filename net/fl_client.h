/*
 * 	fl_client.h
 *
 * 	Client class for flatlands.
 *
 */

#ifndef FL_NET_CLIENT_H_
#define FL_NET_CLIENT_H_

#include <queue>

#include "fl_net.h"

enum FLServerState {
	FL_SERVER_ALIVE,
	FL_SERVER_DEAD,
	FL_SERVER_SELF,
	FL_SERVER_LIMBO,
	FL_SERVER_INVALID
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

		void queue_message( Uint8* data, int len );
	protected:
		void queue_heartbeat();

		void connect_to_server( std::string server_hostname );
		void check_conn();
		void receive();
		void send();
		void handle_packet();
		
		bool initialized;

		std::queue<FLNetMessage*> udp_message_queue;

		UDPpacket* packet;		// used for all received packets
		FLServerConn server_conn;
		UDPsocket socket;
		IPaddress my_ip;
};

#endif

