/*
 *	fl_monster.h
 *
 *	Base class for computer controlled enemies.
 *
 */

#ifndef FL_MONSTER_H_
#define FL_MONSTER_H_

#include "../../rendering/animated_object.h"
#include "../../common/common.h"

enum FLMonsterState {
	FL_MONSTER_IDLE,
	FL_MONSTER_STUNNED,
	FL_MONSTER_ATTACKING,
	FL_MONSTER_MOVING
};

class FLMonster : public FLGameObject {
	public:
		FLMonster( float x, float y, float w, float h, FLAnimatedObjectParams animation_params );

		virtual void per_frame_update();
	protected:
		virtual void attack() {};
		virtual void move() {};
		virtual void on_player_near() {};

		point get_distance_from_player();

		int movement_period;
		int attack_period;
		int stun_duration;
		int movement_tick;
		int attack_tick;
		int stun_tick;

		float vision_radius;
		float near_radius;

		float distance_from_player;
		point vector_from_player;

		bool facing_right;

		FLMonsterState monster_state;
};

#endif

