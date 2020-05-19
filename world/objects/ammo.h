/*
 * 	ammo.h
 *
 * 	Ammo the player can pick up.
 *
 */

#ifndef AMMO_H_
#define AMMO_H_

#include "../../net/net_object.h"
#include "../../rendering/animated_object.h"

class FLAmmo : virtual public FLAnimatedObject, virtual public FLNetObject {
	public:
		FLAmmo( float x, float y, int weapon_index );
		virtual ~FLAmmo();
		virtual void collide_with();
	protected:
		int weapon_index;
		int num_clips;
		
};

#endif

