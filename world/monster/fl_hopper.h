/*
 * 	fl_hooper.h
 *
 * 	a hopping monster
 *
 */

#ifndef FL_HOPPER_H_
#define FL_HOOPER_H_

#include "fl_monster.h"

class FLHopper : public FLMonster {
	public:
		FLHopper(float x, float y);
		
		virtual void per_frame_update() override;
		virtual void hit(float damage) override;
	protected:
		virtual void on_player_near() override;
		virtual void attack() override;
};

#endif

