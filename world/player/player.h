/*
 * 	player.h
 *
 * 	Base player class for a character who is sensitive to physics
 * 	and user input.
 *
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "../../resources/fl_resources.h"
#include "../../rendering/animated_object.h"
#include "../physics_object.h"

class FLWorldSurface;
struct texture;

enum FLPlayerAbility {
	FL_NO_ABILITY,
	FL_DOUBLE_JUMP,
	FL_GROUND_POUND,
	FL_HOVER,
	FL_NUM_ABILITIES
};

enum FLPlayerState {
	FL_PLAYER_IDLE,
	FL_PLAYER_WALK,
	FL_PLAYER_JUMP,
	FL_PLAYER_POUND
};

class FLPlayer : public FLAnimatedObject, public FLPhysicsObject {
	public:
		FLPlayer();

		virtual void update_animation();

		void enable_ability();

		//actions
		void interact();
		void jump();
		virtual void use_ability();
		virtual void set_ability( FLPlayerAbility ability );

		// movement methods
		void move_left();
		void move_right();
		virtual void release_walk();

		virtual void update_physics();
		virtual void update_camera();

		virtual void hold_jump();
		virtual void release_jump();
		virtual void hold_run();
		virtual void release_run();

		void set_reset_position( float x, float y );
		virtual void reset();

		bool pounding();
		bool facing_right();

	protected:
		virtual void apply_gravity();
		virtual void bind_actions();
		virtual void bound_velocity();

		virtual void double_jump();
		virtual void ground_pound();
		virtual void hover();

		bool can_use_ability;
		FLPlayerAbility cur_ability;
		bool jump_held;
		bool run_held;

		unsigned int jump_frames;
		unsigned int hover_frames;
		unsigned int pound_frames;
		unsigned int falling_frames;

		FLPlayerState state;

		point reset_position;
};

#endif

