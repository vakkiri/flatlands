/*
 *	fl_orb.h
 *
 *	A 'monster' that doesn't move or attach. Gives orbs on death.
 *
 */

#ifndef FL_ORB_H_
#define FL_ORB_H_

#include "fl_monster.h"

class FLOrb : public FLMonster {
	public:
		FLOrb(float x, float y);
		virtual ~FLOrb();
		virtual void hit(float damage) override;
		virtual void per_frame_update();
};

#endif

