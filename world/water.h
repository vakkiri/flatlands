/*
 * 	water.h
 *
 * 	Puddlez
 *
 */

#ifndef FL_WATER_H_
#define FL_WATER_H_

#include "../common/game_object.h"
#include "../rendering/animated_object.h"

class FLWater : virtual public FLGameObject {
	public:
		FLWater(float x, float y, float w, float h);
		virtual ~FLWater();
};

void clear_water();

#endif

