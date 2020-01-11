/*
 * 	world_object.h
 *
 * 	Base type of objects which exist in the game world.
 *
 */

#ifndef WORLD_OBJECT_H_
#define WORLD_OBJECT_H_

#include "../common/common.h"

class FLWorldObject {
	public:
		FLWorldObject(){};
		FLWorldObject(float x, float y, float w, float h);
		virtual ~FLWorldObject(){}

		virtual void set_x( float x );
		virtual void set_y( float y );
		virtual float x();
		virtual float y();
		virtual float w();
		virtual float h();
		virtual float bounds_x();
		virtual float bounds_y();
		virtual float bounds_w();
		virtual float bounds_h();
	protected:
		rect position;
};

#endif
