/*
 * 	platform.h
 *
 * 	Moving platforms which the player can stand on.
 *
 */

#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "../../rendering/animated_object.h"
#include "../colliding_object.h"
#include "../dynamic_object.h"

class FLPlatform : virtual public FLCollidingObject, virtual public FLAnimatedObject, virtual public FLDynamicObject {
	public:
		FLPlatform( float x, float y );
		virtual ~FLPlatform();

		virtual void collide_with( FLPlayer *player );
		virtual void update();

	protected:
		point left;
		point right;
		point vel;
		bool moving_right;
};

#endif

