/*
 * 	fl_attack.h
 *
 */

#ifndef FL_ATTACK_H_
#define FL_ATTACK_H_

#include "../../common/game_object.h"

enum FLAttackState {
	FL_ATTACK_PRE,
	FL_ATTACK_ATTACK,
	FL_ATTACK_POST
};

class FLAttack : public FLGameObject {
	public:
		FLAttack() = delete;
		FLAttack(float x, float y, float w, float h, FLHorizontalDirection hdir, FLVerticalDirection vdir);

		virtual void per_frame_update();

	protected:
		FLVerticalDirection vdir;
		FLHorizontalDirection hdir;
		virtual void pre_attack() {};
		virtual void attack() {};
		virtual void post_attack() {};
		int pre_ticks;
		int attack_ticks;
		int post_ticks;
		int tick;
		FLAttackState attack_state;
};

#endif

