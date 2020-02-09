/*
 * 	toggler.h
 *
 * 	Toggles the "toggle tiles" on/off
 *
 */

#ifndef TOGGLER_H_
#define TOGGLER_H_

#include "../../rendering/animated_object.h"
#include "../colliding_object.h"

class FLToggler : virtual public FLCollidingObject, virtual public FLAnimatedObject {
	public:
		FLToggler( float x, float y );
		virtual ~FLToggler();
		virtual void collide_with( FLPlayer *player );

		virtual float y();
		virtual float bounds_h();
		virtual float bounds_y();
		virtual void update_animation();
		
	protected:
		unsigned int cooldown;
};

#endif

