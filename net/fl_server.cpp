/*
 * 	fl_server.cpp
 *
 */

#include <iostream>
#include <utility>

#include "fl_net.h"
#include "fl_server.h"
#include "net_object.h"

#include "../world/player/net_player.h"

// XXX this is bad and inaccurate
#define MS_PER_FRAME (1000.f/60.f)

FLServer::FLServer() {
	initialized = false;
	socket = nullptr;
	last_pos_update = SDL_GetTicks();

	for (int i = 0; i < FL_MAX_CONN; ++i) {
		client_conns[i].accepted = false;
		client_conns[i].state = FL_CLIENT_DISCONNECTED;
		client_conns[i].last_tick = 0;
	}
}

void FLServer::start() {
	if ( SDLNet_ResolveHost( &my_ip, NULL, FL_SERVER_PORT ) == -1 ) {
		std::cout << "Error resolving server host: " << SDLNet_GetError() << std::endl;
		return;
	}

	socket = SDLNet_UDP_Open(FL_SERVER_PORT);

	if ( socket == nullptr ) {
		std::cout << "Unable to open port for server UDP socket.\n";
		return;
	}

	packet = SDLNet_AllocPacket(FL_MAX_PACKET_LEN);

	if ( packet == nullptr ) {
		std::cout << "Unable to create receiving UDP packet\n";
		return;
	}

	std::cout << "Server running on port " << my_ip.port << "\n";
	initialized = true;
}

void FLServer::queue_heartbeats() {
	Uint32 tick = SDL_GetTicks();

	for ( int i = 0; i < FL_MAX_CONN; ++i ) {
		if ( client_conns[i].accepted && client_conns[i].state == FL_CLIENT_CONNECTED ) {
			if ( tick - client_conns[i].last_heartbeat >= FL_HEARTBEAT_INTERVAL ) {
				queue_heartbeat(i);
			}
		}
	}
}

void FLServer::update_player_info( float x, float y, float vx, float vy, int animation ) {
	player_info.x = x;
	player_info.y = y;
	player_info.vx = vx;
	player_info.vy = vy;
	player_info.animation = animation;
}

void FLServer::update_client_positions() {
	// Only update after the appropriate amount of time has passed
	Uint32 tick = SDL_GetTicks();
	Uint32 elapsed_ms = (tick - last_pos_update);
	if ( elapsed_ms >= FL_POS_SEND_INTERVAL ) {
		last_pos_update = tick;
		float elapsed_frames = (elapsed_ms) / MS_PER_FRAME;
		// For every active client, send a message to all other clients with their last position
		// TODO: currently this only sends the server's own position
		for ( int i = 0; i < FL_MAX_CONN; ++i ) {
			if ( client_conns[i].state == FL_CLIENT_CONNECTED ) {
				int16_t x = (int16_t) (player_info.x + (player_info.vx * elapsed_frames));
				int16_t y = (int16_t) player_info.y;
				int len = 7;
				Uint8* data = new Uint8[7];
				data[0] = FL_MSG_POS;
				memcpy(&(data[1]), &x, sizeof(int16_t));
				memcpy(&(data[3]), &y, sizeof(int16_t));
				data[5] = player_info.animation;
				// We use the current slot for a client's connection to us, the server, since
				// the client doesn't need that slot for themself
				data[6] = i;
				queue_message( i, data, len,  false );
			}
		}
	}
}

void FLServer::update_clients() {
	// Send player positions to all clients
	update_client_positions();
}

void FLServer::update() {
	if ( initialized ) {
		queue_heartbeats();
		check_conns();
		update_clients();
		send();
		receive();
	}
}

void FLServer::send() {
	static int frame = 0;
	++frame;
	int sent = 0;
	Uint32 tick = SDL_GetTicks();

	while ( !udp_msg_queue.empty() && sent < MAX_FRAME_SEND ) {
		FLNetMessage* msg = udp_msg_queue.front();
		if ( msg->data[0] == FL_MSG_DEL_OBJ )
			std::cout << "Server sending message.\n";
		fl_send_udp( msg->data, msg->len, msg->dest, socket );
		delete [] msg->data;
		udp_msg_queue.pop();
	}
	while ( !synchronized_msg_queue.empty() && sent < MAX_FRAME_SEND ) {
		++sent;
		if ( synchronized_msg_queue.front() == nullptr ) {
			synchronized_msg_queue.pop();
		}
		else if ( tick - synchronized_msg_queue.front()->last_send >= FL_RESEND_INTERVAL ) {
			std::cout << "Server: Sending synchronized message.\n";
			FLSynchronizedNetMessage* smsg = synchronized_msg_queue.front();
			fl_send_udp( smsg->msg->data, smsg->msg->len, smsg->msg->dest, socket );
			synchronized_msg_queue.pop();
			smsg->last_send = tick;
			synchronized_msg_queue.push(smsg);
		}
	}
}

void FLServer::clear_del_item_synchronized_messages( uint16_t id, uint32_t host ) {
	std::queue<FLSynchronizedNetMessage*> alt_queue;
	uint16_t sid;
	Uint8 stype;
	bool host_matches;
	bool type_matches;
	bool id_matches;

	while ( !synchronized_msg_queue.empty() ) {
		FLSynchronizedNetMessage *smsg = synchronized_msg_queue.front();
		synchronized_msg_queue.pop();

		stype = smsg->msg->data[0];
		memcpy( &sid, &(smsg->msg->data[1]), sizeof(uint16_t) );

		host_matches = (host == 0 || host == smsg->msg->dest.host);
		type_matches = (stype == FL_MSG_DEL_OBJ);
		id_matches = (sid == id);

		if ( host_matches && type_matches && id_matches ) {
			delete smsg->msg->data;
			delete smsg->msg;
			delete smsg;
			smsg = nullptr;
		}
		else {
			alt_queue.push(smsg);
		}
	}

	swap( alt_queue, synchronized_msg_queue );
}


void FLServer::receive() {
	int received = 0;

	while( SDLNet_UDP_Recv(socket, packet) && received < MAX_FRAME_RECV ) {
		handle_packet();
		++received;
	}
}


void FLServer::accept_heartbeat( IPaddress addr ) {
	int slot = get_addr_slot(addr);

	// if we found the client
	if ( slot >= 0 ) {
		// XXX again this isn't totally accurate since we won't process this immediately...
		client_conns[slot].last_tick = SDL_GetTicks();
		client_conns[slot].ping = client_conns[slot].last_tick - client_conns[slot].last_heartbeat;
	}
}


int FLServer::get_addr_slot(IPaddress addr) {
	int slot;

	for ( slot = 0; slot < FL_MAX_CONN; ++slot ) {
		if (client_conns[slot].ip.host == addr.host)
			break;
	}

	if ( slot >= FL_MAX_CONN ) {
		slot = -1;
	}

	return slot;
}


void FLServer::update_client_pos( IPaddress addr, Uint8* data ) {
	int slot;
	int16_t x;
	int16_t y;
	Uint8 animation = data[5];

	slot = get_addr_slot( addr );

	if ( slot >= 0 && client_conns[slot].player != nullptr ) {
		bool reverse = animation & ANIM_REVERSE_BIT;
		animation &= (ANIM_REVERSE_BIT - 1);
		memcpy(&x, &(data[1]), sizeof(int16_t));
		memcpy(&y, &(data[3]), sizeof(int16_t));

		client_conns[slot].player->set_reverse(reverse);
		client_conns[slot].player->set_target((float) x, (float) y);
		client_conns[slot].player->set_animation(data[5]);
	}

}


void FLServer::ack_del_obj( IPaddress addr, Uint8* data ) {
	int slot;
	int obj_id;

	slot = get_addr_slot( addr );

	// Send the acknowledgement back to the sender
	if ( slot >= 0 && client_conns[slot].player != nullptr ) {

		// TODO ? it would be cleaner in these situations to just have a struct type for memcpy
		Uint8* out_data = new Uint8(3);
		data[0] = FL_MSG_ACK_DEL_OBJ;
		memcpy( &(out_data[1]), &(data[1]), sizeof(uint16_t) );
		memcpy( &obj_id, &(data[1]), sizeof(uint16_t) );

		queue_message( slot, out_data, 3, false );

		// delete the object from the server
		del_net_obj( obj_id );
	}
}


void FLServer::handle_ack_del_obj( IPaddress addr, Uint8* data ) {
	std::cout << "Server: deletion acknowledged\n";
	int slot = get_addr_slot( addr );
	uint16_t obj_id;
	if ( slot >= 0 ) {
		memcpy( &obj_id, &(data[1]), sizeof(uint16_t) );
		mark_net_object_deleted( obj_id, slot );
		clear_del_item_synchronized_messages( obj_id, addr.host );
	}
}


void FLServer::sync_del_obj( uint16_t id ) {
	// This should be called *only* when an object is deleted by the server.
	// It is a one-time generation of all the necessary synchronized messages for the object deletion.
	// This method assumes that we have already filled in an entry in deleted_net_objects for the object.

	create_obj_del_sync_table( id );

	auto it = deleted_net_objects.find( id );
	if ( it != deleted_net_objects.end() ) {
		for ( int i = 0; i < FL_MAX_CONN; ++i ) {
			if ( it->second[i] == false ) {
				Uint8* data = new Uint8[3];
				data[0] = FL_MSG_DEL_OBJ;
				memcpy( &(data[1]), &id, sizeof( uint16_t ));
				queue_message( i, data, 3, true );
			}
			// else the client is not connected
		}
	}
	else {
		std::cout << "ERROR: Deletion sync table entry for item not found.\n";
	}
}


void FLServer::create_obj_del_sync_table( uint16_t id ) {
	if ( deleted_net_objects.find(id) == deleted_net_objects.end() ) {
		deleted_net_objects[id] = std::vector<bool>( FL_MAX_CONN, false );

		// Mark any inactive clients as synchronized so we don't wait for them
		for ( int i = 0; i < FL_MAX_CONN; ++i ) {
			if ( client_conns[i].state == FL_CLIENT_DISCONNECTED ) {
				deleted_net_objects[id][i] = true;
			}
		}
	}
}


void FLServer::mark_net_object_deleted( uint16_t id, int client_slot ) {
	auto it = deleted_net_objects.find( id );
	if ( client_slot >= 0 && it != deleted_net_objects.end() ) {
		bool synchronized = true;
		deleted_net_objects[id][client_slot] = true;
		for ( int i = 0; i < FL_MAX_CONN; ++i ) {
			if ( deleted_net_objects[id][i] == false ) {
				synchronized = false;
				break;
			}
		}
		// If all clients are synchronized, we can remove the entry
		if ( synchronized ) {
			deleted_net_objects.erase( it );
			clear_del_item_synchronized_messages( id, 0 );	// 0 indicates to delete from any host
		}
		else {
			std::cout << " not synchronized yet for id " << id << std::endl;
		}
	}
	// Else, we received a message from a client about an item which does not exist
}


void FLServer::accept_client_conn( IPaddress addr ) {
	// open a socket with the client
	// add to client_sockets
	// Find avalable slot
	FLClientConn* conn;

	int avail_slot = -1;

	for ( int i = 0; i < FL_MAX_CONN; ++i ) {
		if ( client_conns[i].ip.host == addr.host ) {
			reconnect_client(i);
			avail_slot = -1;
			break;
		}
		else if ( !client_conns[i].accepted ) {
			avail_slot = i;
			break;
		}
	}

	if ( avail_slot >= 0 ) {
		conn = &(client_conns[avail_slot]);

		conn->accepted = true;
		conn->ip = addr;
		conn->last_tick = SDL_GetTicks();
		conn->state = FL_CLIENT_CONNECTED;
		if ( conn->player != nullptr ) {
			delete conn->player;
		}
		conn->player = new FLNetPlayer();


		std::cout << "accepted a connection in slot " << avail_slot << std::endl;

		queue_heartbeat(avail_slot);
	}
}


void FLServer::queue_message( int slot, Uint8* data, int len, bool synchronized ) {
	FLNetMessage* msg = new FLNetMessage;

	msg->data = data;
	msg->len = len;
	msg->dest = client_conns[slot].ip;

	if ( !synchronized ) {
		udp_msg_queue.push(msg);
	}
	else {
		FLSynchronizedNetMessage* smsg = new FLSynchronizedNetMessage;
		smsg->msg = msg;
		smsg->last_send = 0;

		// Set what type of message confirms synchronization
		switch ( data[0] ) {
			case FL_MSG_DEL_OBJ:
				smsg->ack_type = FL_MSG_ACK_DEL_OBJ;
				break;
			default:
				smsg->ack_type = FL_MSG_UNK;
				break;
		}

		// Don't follow through if we don't have a valid acceptance type;
		// this would cause the message to repeat infinitely

		if ( smsg->ack_type != FL_MSG_UNK ) {
			synchronized_msg_queue.push(smsg);
		}
		else {
			std::cout << "Server: ERROR tried to create sync message with unknown response type.\n";
			delete msg->data;
			delete msg;
			delete smsg;
		}
	}
}


void FLServer::queue_heartbeat( int slot ) {
	Uint8* data = new Uint8[1];
	*data = FL_MSG_HEARTBEAT;

	client_conns[slot].last_heartbeat = SDL_GetTicks();

	queue_message( slot, data, 1, false );
}


void FLServer::reconnect_client( int slot ) {
	// This is where we might do some kinda of game world state update..
	// Send a heartbeat
	client_conns[slot].accepted = true;
	client_conns[slot].last_tick = SDL_GetTicks();
	queue_heartbeat(slot);
}


void FLServer::check_conns() {
	Uint32 tick = SDL_GetTicks();

	for (int i = 0; i < FL_MAX_CONN; ++i) {
		if (client_conns[i].accepted) {	// we only care about active slots
			if (client_conns[i].state == FL_CLIENT_CONNECTED) {
				if (tick - client_conns[i].last_tick >= FL_TIMEOUT) {
					client_conns[i].state = FL_CLIENT_DISCONNECTED;
					std::cout << "Server: Client " << client_conns[i].ip.host << " disconnected\n";
				}
			}
			// here we could handle tryng to reconnect...
		}
	}
}


void FLServer::handle_packet() {
	if ( packet->len < FL_MIN_PACKET_LEN || packet->len > FL_MAX_PACKET_LEN ) {
		std::cout << "Error: invalid packet size.\n";
		return;
	}

	Uint8* data = new Uint8[packet->len];
	memcpy(data, packet->data, packet->len);

	switch ( data[0] ) {
		case FL_MSG_HEARTBEAT:
			accept_heartbeat(packet->address);
			break;
		case FL_MSG_CONN:
			accept_client_conn(packet->address);
			break;
		case FL_MSG_POS:
			update_client_pos(packet->address, packet->data);
			break;
		case FL_MSG_DEL_OBJ:
			ack_del_obj(packet->address, packet->data);
			break;
		case FL_MSG_ACK_DEL_OBJ:
			handle_ack_del_obj(packet->address, packet->data);
			break;
		default:
			std::cout << "Server: Unknown message received.\n";
			break;
	}

	delete [] data;
}

