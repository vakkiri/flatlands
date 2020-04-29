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
	IPaddress dest;
};

struct FLMsgPos {
	float x;
	float y;
};

void fl_send_udp(Uint8 *data, int data_len, IPaddress addr, UDPsocket socket);

#endif

