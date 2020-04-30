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

class FLNetPlayer;

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
	FLNetPlayer* player;
};

struct FLServerPlayerInfo {
	float x;
	float y;
	float vx;
	float vy;
	int animation;
};

class FLServer {
	public:
		FLServer();
		void start();
		void update();

		void update_player_info( float x, float y, int animation );
	protected:
		void queue_message(int slot, Uint8* data, int len);

		void queue_heartbeats();
		void queue_heartbeat(int slot);

		void update_clients();
		void update_client_positions();

		void check_conns();
		void receive();
		void send();
		void handle_packet();
		void accept_heartbeat(IPaddress addr);
		void reconnect_client(int slot);

		int get_addr_slot(IPaddress addr);

		// Message handling
		void accept_client_conn(IPaddress addr);
		void update_client_pos(IPaddress addr, Uint8* data);

		bool initialized;

		std::queue<FLNetMessage*> udp_message_queue;

		UDPpacket* packet;	// used for all received packets

		IPaddress my_ip;
		FLClientConn client_conns[FL_MAX_CONN];
		UDPsocket socket;

		FLServerPlayerInfo player_info;
		Uint32 last_pos_update;
};

#endif

