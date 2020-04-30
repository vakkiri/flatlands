/*
 * 	fl_net.h
 *
 * 	flatlands networking header
 *
 */

#ifndef FL_NET_H_
#define FL_NET_H_

#include <SDL2/SDL_net.h>
#include "common.h"
#include "glo.h"

bool fl_init_net();
void fl_start_server();
void fl_start_client();
void fl_update_server();
void fl_update_client();

void send_udp_to_server(int message_type, void* data);
void update_server_player_info( float x, float y, int animation );

#endif
