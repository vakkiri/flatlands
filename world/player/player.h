/*
 * 	player.h
 *
 * 	Base player class for a character
 * 	and user input.
 *
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "../../common/common.h"
#include "../../net/fl_net.h"
#include "../../resources/fl_resources.h"

class FLAnimatedObject;
class FLWorldSurface;
class FLCollider;

struct texture;

enum FLPlayerAbility { FL_NO_ABILITY, FL_DASH };

enum FLPlayerAnimationState {
	FL_PLAYER_IDLE,
	FL_PLAYER_WALK,
	FL_PLAYER_JUMP,
	FL_PLAYER_FALL,
	FL_PLAYER_WALL,
	FL_PLAYER_DASH,
};

enum FLPlayerAnimationAction {
	FL_PLAYER_JUMP_ACTION,
	FL_PLAYER_FALL_ACTION,
	FL_PLAYER_ATTACK_ACTION,
	FL_PLAYER_RUN_ACTION,
	FL_PLAYER_DASH_ACTION,
	FL_PLAYER_STOP_ACTION
};

enum FLPlayerWeapon { FL_NO_WEAPON, FL_FUSION, FL_NUM_WEAPONS };

struct FLWeaponStats {
	int ammo;
	int clip_size;
	float recoil;
	bool posessed;
};

class FLPlayer : public FLGameObject {
  public:
	FLPlayer();
	virtual ~FLPlayer();

	void enable_ability();

	// actions
	void interact();
	void jump();
	virtual void use_ability();
	virtual void attack();
	virtual void stop_attack();
	virtual void set_ability(FLPlayerAbility ability);
	virtual void per_frame_update();
	virtual void animation_update();

	// movement methods
	void move_left();
	void move_right();
	void release_left();
	void release_right();

	void reset_camera();

	virtual void hold_jump();
	virtual void release_jump();
	virtual void press_up();
	virtual void press_down();
	virtual void release_up();
	virtual void release_down();

	virtual void hit_ground();

	void set_reset_position(float x, float y);
	virtual void reset();

	bool pounding();
	bool dashing();
	bool running();
	bool can_dash();
	bool can_attack();
	bool facing_right();

	virtual void drain_ammo();
	virtual void add_ammo(int weapon, int num_clips);

	virtual void hit(int damage);

	float health_ratio();
	float clip_ratio();

	int get_chips();
	int get_fragments();
	int get_ammo();
	void add_chip();
	void add_fragment();
	bool attacking();
  protected:
	virtual void handle_collision(FLCollider *collision);
	virtual void transition_animation(FLPlayerAnimationAction action);

	virtual void update_net();
	virtual void update_health();

	virtual void bind_actions();

	virtual void double_jump();
	virtual void ground_pound();
	virtual void dash();

	virtual void init_weapon_stats();

	bool wall_sliding();
	bool can_wall_jump();

	FLPlayerAbility cur_ability;
	FLPlayerWeapon cur_weapon;
	FLVerticalDirection vertical_direction;
	FLWeaponStats weapon_stats[FL_NUM_WEAPONS];
	FLPlayerAnimationState animation_state;

	int max_health;
	int health;
	int target_health;
	int chips;
	int fragments;

	bool can_use_ability;
	bool can_double_jump;
	bool right_held;
	bool left_held;
	bool up_held;
	bool down_held;
	bool jump_held;

	unsigned int dash_frames;
	unsigned int pound_frames;
	unsigned int falling_frames;
	unsigned int invulnerable_frames;
	unsigned int post_attack_timer;
	bool attack_held;
	int wall_jump_frames;

	point reset_position;

	// Networking
	Uint32 last_update_tick;
	FLMsgPos net_pos;
};

#endif
