/*
 * 	pound_powerup.h
 *
 * 	Powerup which gives the player the ground pound ability.
 *
 */

#ifndef POUND_POWERUP_H_
#define POUND_POWERUP_H_

#include "../../rendering/animated_object.h"
#include "../colliding_object.h"

class FLPlayer;

class FLPoundPowerup : virtual public FLAnimatedObject, virtual public FLCollidingObject {
	public:
		FLPoundPowerup( float x, float y );
		virtual ~FLPoundPowerup();
		virtual void collide_with( FLPlayer *player );
		virtual void update_animation();
};

#endif

