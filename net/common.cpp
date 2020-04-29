/*
 * 	net/common.cpp
 *
 */

#include <iostream>
#include "fl_net.h"
#include "common.h"

void fl_send_udp(Uint8 *data, int data_len, IPaddress addr, UDPsocket socket) {
	if (data_len < FL_MIN_PACKET_LEN || data_len > FL_MAX_PACKET_LEN) {
		std::cout << "Could not send packet: invald size\n";
		return;
	}

	UDPpacket *packet = SDLNet_AllocPacket(data_len);

	if (packet == nullptr) {
		std::cout << "Could not allocate packet to send.\n";
		return;
	}

	memcpy(packet->data, data, data_len);
	packet->len = data_len;
	packet->address.host = addr.host;
	packet->address.port = addr.port;
	
	if ( SDLNet_UDP_Send(socket, -1, packet) == 0 ) {
		std::cout << "UDP send failed: " << SDLNet_GetError() << std::endl;
	}

	SDLNet_FreePacket(packet);
}

