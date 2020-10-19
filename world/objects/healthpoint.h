/*
 * 	healthpoint.h
 *
 * 	Spots to recharge health.
 *
 */

#ifndef HEALTHPOINT_H_
#define HEALTHPOINT_H_

#include "../../common/game_object.h"

class FLHealthPoint : virtual public FLGameObject {
	public:
		FLHealthPoint(float x, float y);
		virtual ~FLHealthPoint();
		virtual void interact();
};

void clear_healthpoints();

#endif

