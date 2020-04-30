/*
 * 	net/glo.h
 *
 *	Global definitions for the network module.
 *
 */

#ifndef FL_NET_GLO_H_
#define FL_NET_GLO_H_


#define FL_MAX_CONN		16
#define MAX_FRAME_RECV		10		// handle up to this many packets per frame

#define FL_SERVER_PORT		8099
#define FL_CLIENT_PORT		8098

#define FL_TIMEOUT		5000		// timeout after this many ms without activity
#define FL_HEARTBEAT_INTERVAL	2000		// how often we send a heartbeat
#define FL_POS_SEND_INTERVAL	100		// how often in ms we update the player's position


#define FL_MIN_PACKET_LEN	1
#define FL_MAX_PACKET_LEN	10

#define FL_MSG_HEARTBEAT	0x00
#define FL_MSG_CONN		0x01
#define FL_MSG_POS		0x02


#endif

