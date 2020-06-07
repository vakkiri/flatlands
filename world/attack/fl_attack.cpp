/*
 * 	fl_attack.cpp
 *
 */

#include "fl_attack.h"
#include "../../components/components.h"

FLAttack::FLAttack(float x, float y, float w, float h) : FLGameObject(x, y, w, h) {
	updator_handle = new_updator(this);
	attack_state = FL_ATTACK_PRE;
	pre_ticks = 0;
	attack_ticks = 0;
	post_ticks = 0;
}

void FLAttack::per_frame_update() {
	switch (attack_state) {
	case FL_ATTACK_PRE: {
		pre_attack();
		if (tick >= pre_ticks) {
			attack_state = FL_ATTACK_ATTACK;
			tick = 0;
		}
		break;
			    }
	case FL_ATTACK_ATTACK: {
		attack();
		if (tick >= attack_ticks) {
			attack_state = FL_ATTACK_POST;
			tick = 0;
		}
		break;
			       }
	case FL_ATTACK_POST: {
		post_attack();
		if (tick >= attack_ticks) {
			delete this;
		}
		break;
			     }
	default: {
		break;
		 }
	}
	
	++tick;
}

