/*
 * 	colliding_object.h
 *
 * 	Objects which perform some sort of action when the player collides
 * 	with them.
 *
 */

#ifndef COLLIDING_OBJECT_H_
#define COLLIDING_OBJECT_H_

#include "world_object.h"

class FLPlayer;

class FLCollidingObject : virtual public FLWorldObject {
	public:
		virtual ~FLCollidingObject(){}
		virtual void collide_with( FLPlayer *player ) = 0;
};

#endif

