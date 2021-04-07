/*
 * 	net_player.h
 *
 * 	Player objects for networked players.
 *
 */

#ifndef FL_NET_PLAYER_H_
#define FL_NET_PLAYER_H_

#include "../../rendering/animated_object.h"
#include "../../common/game_object.h"

class FLNetPlayer : public FLGameObject {
  public:
	FLNetPlayer();

	void set_target(float tx, float ty);
	virtual void per_frame_update();

  private:
	point vel;
	point target;
	uint32_t last_position_update;
};

#endif
