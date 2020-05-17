/*
 *	fl_monster.h
 *
 *	Base class for computer controlled enemies.
 *
 */

#ifndef FL_MONSTER_H_
#define FL_MONSTER_H_

#include "../dynamic_object.h"
#include "../physics_object.h"
#include "../world_object.h"
#include "../../rendering/animated_object.h"

enum FLMonsterState {
	FL_MONSTER_IDLE,
	FL_MONSTER_STUNNED,
	FL_MONSTER_ATTACKING,
	FL_MONSTER_MOVING
};

class FLMonster : public FLAnimatedObject, public FLPhysicsObject, public FLDynamicObject {
	public:
		FLMonster( float x, float y, float w, float h, FLAnimatedObjectParams animation_params );

		virtual void update() = 0;
	protected:
		virtual void move() {};
		virtual void attack() {};

		int movement_period;
		int stun_duration;
		bool facing_right;

		FLMonsterState monster_state;
};

#endif

