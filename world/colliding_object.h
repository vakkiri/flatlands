/*
 * 	colliding_object.h
 *
 * 	Objects which perform some sort of action when the player collides
 * 	with them.
 *
 */

#ifndef COLLIDING_OBJECT_H_
#define COLLIDING_OBJECT_H_

#include <vector>
#include "../common/common.h"

class FLPlayer;

class FLCollidingObject : virtual public FLGameObject {
	public:
		FLCollidingObject();
		virtual ~FLCollidingObject();
		virtual void collide_with() = 0;
	protected:
		unsigned int colliding_object_list_pos;
};

void clear_colliding_objects();
std::vector<FLCollidingObject*>& get_colliding_objects();

#endif

