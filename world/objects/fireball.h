/*
 * 	fireball.h
 *
 * 	A moving fireball which will hurt the player.
 *
 */

#ifndef FIREBALL_H_
#define FIREBALL_H_

#include "../../rendering/animated_object.h"
#include "../colliding_object.h"
#include "../dynamic_object.h"

class FLFireball : virtual public FLCollidingObject, virtual public FLAnimatedObject, virtual public FLDynamicObject {
	public:
		FLFireball( float x, float y );
		virtual ~FLFireball();
		virtual void collide_with();
		virtual float x();
		virtual float y();
		virtual void update();
	protected:
		float movement_radius;
		float speed;
		float phase;
		unsigned int offset;
		unsigned int last_tick;
		unsigned int cooldown;
};


class FLSmallball : virtual public FLCollidingObject, virtual public FLAnimatedObject, virtual public FLDynamicObject {
	public:
		FLSmallball( float x, float y );
		virtual ~FLSmallball();
		virtual void collide_with();

		virtual void update();
		void set_velocity( float x, float y );
	protected:
		point velocity;
		unsigned int life;
};

#endif

