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

		float x();
		float y();
		float w();
		float h();
		float s();
		float t();
		virtual float bounds_x();
		virtual float bounds_y();
		virtual float bounds_w();
		virtual float bounds_h();

		void set_st( float s, float t );
	private:
		rect position;
		float _s;
		float _t;

};

#endif
