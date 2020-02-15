/*
 * 	world_object.h
 *
 * 	Base type of objects which exist in the game world.
 *
 */

#ifndef WORLD_OBJECT_H_
#define WORLD_OBJECT_H_

#include "../common/common.h"

class FLWorldObject : virtual public FLGameObject {
	public:
		FLWorldObject();
		FLWorldObject(float x, float y, float w, float h);
		virtual ~FLWorldObject();

		virtual float bounds_x();
		virtual float bounds_y();
		virtual float bounds_w();
		virtual float bounds_h();

	protected:
		unsigned int wo_list_position;
};

void clear_world_objects();

#endif
