/*
 * 	fl_client.h
 *
 * 	Client class for flatlands.
 *
 */

#ifndef FL_NET_CLIENT_H_
#define FL_NET_CLIENT_H_

#include <string>
#include <queue>

#include "fl_net.h"

class FLNetPlayer;

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
	~FLClient();
	void start();
	void update();

	// Message factories
	void fill_pos_message(void *data, FLNetMessage *msg);
	void fill_del_obj_message(void *data, FLNetMessage *msg);
	void fill_ack_del_obj_message(uint16_t id, FLNetMessage *msg);

	// Message queueing
	void queue_message(FLNetMessage *msg, bool synchronized);

  protected:
	void queue_heartbeat();

	void connect_to_server(std::string server_hostname);
	void update_players();
	void check_conn();
	void receive();
	void send();
	void handle_packet();
	void ack_synchronized_message(Uint8 ack_type, uint16_t id, Uint32 host);

	// Message handling
	void update_player_pos(int slot, float x, float y, int animation);

	// Variables
	bool initialized;

	std::queue<FLNetMessage *> udp_msg_queue;
	std::queue<FLSynchronizedNetMessage *> synchronized_msg_queue;

	UDPpacket *packet; // used for all received packets
	FLServerConn server_conn;
	UDPsocket socket;
	IPaddress my_ip;

	FLNetPlayer *net_players[FL_MAX_CONN];
};

#endif
