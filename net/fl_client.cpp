/*
 * 	fl_client.cpp
 *
 */

#include <iostream>
#include "fl_client.h"
#include "../world/player/net_player.h"

FLClient::FLClient() {
	initialized = false;
	socket = nullptr;

	server_conn.last_tick = 0;
	server_conn.state = FL_SERVER_DEAD;

	for ( int i = 0; i < FL_MAX_CONN; ++i ) {
		net_players[i] = nullptr;
	}
}

FLClient::~FLClient() {
	if ( initialized ) {
		for ( int i = 0; i < FL_MAX_CONN; ++i ) {
			if ( net_players[i] != nullptr ) {
				delete net_players[i];
			}
		}
	}
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
	if ( SDLNet_ResolveHost( &server_conn.ip, server_hostname.c_str(), FL_SERVER_PORT) == -1 ) {
		std::cout << "Client was uneable to resolve Server hostname.\n";
		server_conn.state = FL_SERVER_INVALID;
		return;
	}

	Uint8 data = 1;
	std::cout << "Client: Server not connected, placed in limbo state.\n";
	server_conn.state = FL_SERVER_LIMBO;
	server_conn.last_tick = SDL_GetTicks();
	fl_send_udp( &data, 1, server_conn.ip, socket );
}

void FLClient::update_players() {
	for ( int i = 0; i < FL_MAX_CONN; ++i ) {
		if ( net_players[i] != nullptr ) {
			net_players[i]->update();
		}
	}
}

void FLClient::update() {
	if ( initialized ) {
		check_conn();
		update_players();
		send();
		receive();
	}
}

void FLClient::queue_message( FLNetMessage* msg, bool synchronized ) {
	if ( !synchronized ) {
		udp_msg_queue.push(msg);
	}
	else {
		FLSynchronizedNetMessage* smsg = new FLSynchronizedNetMessage;
		smsg->msg = msg;
		smsg->last_send = 0;
		synchronized_msg_queue.push_back(msg);
	}
}

void FLClient::queue_heartbeat() {
	FLNetMessage* msg = new FLNetMessage;
	msg->len = 1;
	msg->data = new Uint8[1];
	*(msg->data) = FL_MSG_HEARTBEAT;
	msg->dest = server_conn.ip;
	queue_message( msg, false );
}

void FLClient::send() {
	static int frame = 0;
	++frame;
	int sent = 0;
	Uint32 tick = SDL_GetTicks();

	while ( !udp_msg_queue.empty() && sent < MAX_FRAME_SEND) {
		FLNetMessage* msg = udp_msg_queue.front();
		fl_send_udp( msg->data, msg->len, msg->dest, socket );
		delete msg->data;
		udp_msg_queue.pop();
	}
	/*
	while ( !udp_msg_queue.empty() && sent < MAX_FRAME_SEND) {
		FLSynchronizedNetMessage* msg = synchronized_msg_queue.front();
		if (synchronized_msg_queue[i] == nullptr) {
			// I need to fix this, the vector should be of synchronized messages not just messages
			synchronized_msg_queue[i] = synchronized_msg_queue.back();
		}
	}
	*/
}

void FLClient::receive() {
	int received = 0;
	while ( SDLNet_UDP_Recv(socket, packet) && received < MAX_FRAME_RECV ) {
		handle_packet();
		++received;
	}
}

void FLClient::check_conn() {
	Uint32 tick = SDL_GetTicks();

	switch ( server_conn.state ) {
		case FL_SERVER_DEAD:
			connect_to_server( "10.0.0.81" );
			break;
		case FL_SERVER_ALIVE:
			if ( tick - server_conn.last_tick >= FL_TIMEOUT ) {
				server_conn.state = FL_SERVER_DEAD;
				std::cout << "Client: Disconnected from server.\n";
			}
			break;
		case FL_SERVER_SELF:
			break;
		case FL_SERVER_LIMBO:
			if ( SDL_GetTicks() - server_conn.last_tick > FL_TIMEOUT ) {
				server_conn.state = FL_SERVER_DEAD;
			}
			break;
		case FL_SERVER_INVALID:
			break;
		default:
			break;
	}
}

void FLClient::handle_packet() {
	if ( packet->len < FL_MIN_PACKET_LEN || packet->len > FL_MAX_PACKET_LEN ) {
		std::cout << "Error: invalid packet size.\n";
		return;
	}

	Uint8* data = new Uint8[packet->len];
	memcpy(data, packet->data, packet->len);

	switch ( data[0] ) {
		case FL_MSG_HEARTBEAT:
			server_conn.state = FL_SERVER_ALIVE;
			server_conn.last_tick = SDL_GetTicks();
			queue_heartbeat();
			break;
		case FL_MSG_CONN:
			server_conn.state = FL_SERVER_ALIVE;
			break;
		case FL_MSG_POS:
			Uint8 animation, slot;
			int16_t x, y;

			memcpy(&x, &(data[1]), sizeof(int16_t));
			memcpy(&y, &(data[3]), sizeof(int16_t));
			animation = data[5];
			slot = data[6];
			
			update_player_pos(slot, (float) x, (float) y, animation);
			break;
		default:
			std::cout << "Client: Unknown message received.\n";
			break;
	}

	delete data;
}

void FLClient::update_player_pos( int slot, float x, float y, int animation ) {
	bool reverse = animation & ANIM_REVERSE_BIT;
	animation &= (ANIM_REVERSE_BIT - 1);
		
	if ( net_players[slot] == nullptr ) {
		net_players[slot] = new FLNetPlayer();
	}

	net_players[slot]->set_reverse(reverse);
	net_players[slot]->set_target(x, y);
	net_players[slot]->set_animation(animation);
}

void FLClient::fill_pos_message( void *data, FLNetMessage *msg ) {
	int16_t x;
	int16_t y;
	int len;

	FLMsgPos* in_data = (FLMsgPos*) data;
	Uint8* msg_data = new Uint8[6];
	msg->data = msg_data;

	x = (int16_t) in_data->x;
	y = (int16_t) in_data->y;
	
	msg->len = 6;
	msg_data[0] = FL_MSG_POS;
	memcpy( &(msg_data[1]), &x, sizeof(int16_t));
	memcpy( &(msg_data[3]), &y, sizeof(int16_t));
	msg_data[5] = in_data->animation;

	msg->dest = server_conn.ip;
}

