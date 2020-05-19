/*
 * 	net_player.h
 *
 * 	Player objects for networked players.
 *
 */

#ifndef FL_NET_PLAYER_H_
#define FL_NET_PLAYER_H_

#include "../../rendering/animated_object.h"

class FLNetPlayer : public FLAnimatedObject {
	public:
		FLNetPlayer();

		void set_target( float tx, float ty );
		virtual void per_frame_update();

	private:
		point vel;
		point target;
		uint32_t last_position_update;
		bool reverse_last_frame;
};

#endif

