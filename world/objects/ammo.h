/*
 * 	ammo.h
 *
 * 	Ammo the player can pick up.
 *
 */

#ifndef AMMO_H_
#define AMMO_H_

#include "../../rendering/animated_object.h"
#include "../colliding_object.h"

class FLAmmo : virtual public FLCollidingObject, virtual public FLAnimatedObject {
	public:
		FLAmmo( float x, float y, int weapon_index );
		virtual ~FLAmmo();
		virtual void collide_with( FLPlayer *player );
	protected:
		int weapon_index;
		int amt;
		
};

#endif

