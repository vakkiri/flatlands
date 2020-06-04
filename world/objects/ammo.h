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

class FLCollider;

class FLAmmo : virtual public FLGameObject, virtual public FLNetObject {
  public:
	FLAmmo(float x, float y, int weapon_index);
	virtual ~FLAmmo();
	void on_collision(FLCollider *obj);

  protected:
	int weapon_index;
	int num_clips;
};

#endif
