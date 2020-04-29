/*
 * 	player.h
 *
 * 	Base player class for a character who is sensitive to physics
 * 	and user input.
 *
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "../../net/fl_net.h"
#include "../../resources/fl_resources.h"
#include "../../rendering/animated_object.h"
#include "../physics_object.h"

class FLWorldSurface;
struct texture;

enum FLPlayerAbility {
	FL_NO_ABILITY,
	FL_DASH
};

enum FLPlayerState {
	FL_PLAYER_IDLE,
	FL_PLAYER_WALK,
	FL_PLAYER_JUMP,
	FL_PLAYER_POUND,
	FL_PLAYER_DASH
};

enum FLPlayerWeapon {
	FL_NO_WEAPON,
	FL_FUSION,
	FL_NUM_WEAPONS
};

struct FLWeaponStats {
	int ammo;
	float recoil;
	int posessed;
};

class FLPlayer : public FLAnimatedObject, public FLPhysicsObject {
	public:
		FLPlayer();
		virtual ~FLPlayer();

		virtual void update_animation();

		void enable_ability();

		//actions
		void interact();
		void jump();
		virtual void use_ability();
		virtual void attack();
		virtual void stop_attack();
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

		virtual void hit_ground();

		void set_reset_position( float x, float y );
		virtual void reset();

		bool pounding();
		bool dashing();
		bool can_dash();
		bool can_attack();
		bool facing_right();

		FLAnimatedObject* get_weapon();
		virtual void drain_ammo();
		virtual void add_ammo( int weapon, int amount );
	protected:
		virtual void update_net();

		virtual void apply_gravity();
		virtual void bind_actions();
		virtual void bound_velocity();

		virtual void double_jump();
		virtual void ground_pound();
		virtual void dash();

		virtual void init_weapon_stats();

		FLPlayerAbility cur_ability;
		FLPlayerWeapon	cur_weapon;
		FLPlayerState state;

		FLAnimatedObject* weapon;
		FLWeaponStats weapon_stats[FL_NUM_WEAPONS];

		bool can_use_ability;
		bool can_double_jump;
		bool jump_held;
		bool run_held;
		bool dash_right;
		bool attacking;

		unsigned int jump_frames;
		unsigned int dash_frames;
		unsigned int pound_frames;
		unsigned int falling_frames;

		point reset_position;

		// Networking
		Uint32 last_update_tick;
		FLMsgPos net_pos;
};

#endif

