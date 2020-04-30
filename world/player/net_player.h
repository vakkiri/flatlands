/*
 * 	net_player.h
 *
 * 	Player objects for networked players.
 *
 */

#ifndef FL_NET_PLAYER_H_
#define FL_NET_PLAYER_H_

#include "../dynamic_object.h"
#include "../../rendering/animated_object.h"

class FLNetPlayer : public FLAnimatedObject, public FLDynamicObject {
	public:
		FLNetPlayer();

		void set_target( float tx, float ty );
		virtual void update();
	private:
		point vel;
		point target;
};

#endif

