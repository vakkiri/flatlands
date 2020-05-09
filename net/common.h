/*
 * 	net/common.h
 *
 * 	common networking functions
 *
 */

#ifndef _FL_NET_COMMON_H_
#define _FL_NET_COMMON_H_

#include <SDL2/SDL_net.h>

struct FLNetMessage {
	Uint8* data;
	int len;
	IPaddress dest;		// TODO: this should be rennamed to addr as it can be either an origin or a dest
};

/* SynchronizedNetMessage:
 *
 * Synchronized messages continually resend themselves until they receive an identical message from whoever
 * the message was sent to.
 *
 * For example, a synchronized message to client 1 to delete item with id 52 will repeat until we receive
 * a message to delete the same item from client 1. It does not matter if that message was a direct response to
 * us or if another client happened to send the message independently.
 *
 */

struct FLSynchronizedNetMessage {
	FLNetMessage* msg;
	Uint8 ack_type;
	Uint32 last_send;
};

struct FLMsgPos {
	float x;
	float y;
	Uint8 animation;
};

struct FLMsgDelItem {
	uint16_t id;
};

void fl_send_udp(Uint8 *data, int data_len, IPaddress addr, UDPsocket socket);

#endif

