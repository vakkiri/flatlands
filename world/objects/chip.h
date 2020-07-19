/*
 * 	chip.h
 *
 * 	Chips that can be used as currency.
 *
 */

#ifndef CHIP_H_
#define CHIP_H_

#include "../../common/game_object.h"
#include "../../net/net_object.h"
#include "../../rendering/animated_object.h"

class FLCollider;

class FLChip : virtual public FLGameObject, virtual public FLNetObject {
	public:
		FLChip(float x, float y);
		virtual ~FLChip();
		void on_collision(FLCollider *obj);
};

#endif

