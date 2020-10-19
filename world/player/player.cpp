/*
 * 	player.cpp
 *
 */

#include <iostream>
#include <stdlib.h>
#include <unordered_set>

#include "player.h"

#include "../../audio/fl_audio.h"
#include "../../components/components.h"
#include "../../input/input_handler.h"
#include "../../logging/logging.h"
#include "../../rendering/animated_object.h"
#include "../../rendering/renderer.h"
#include "../../rendering/world_surface.h"
#include "../attack/fl_player_attacks.h"
#include "../effect.h"
#include "../physics_settings.h"
#include "../monster/fl_monster.h"
#include "../projectiles/fl_projectiles.h"

#define MAX_FALL (180)
#define INITIAL_WALK_ACCEL (0.39)
#define WALK_ACCEL (0.4)
#define DASH_INITIAL_ACCEL (5.0)
#define DASH_ACCEL (0.35)

#define JUMP_ACCEL (-5)
#define DOUBLE_JUMP_ACCEL (-4.3)
#define WALL_JUMP_Y (-3)
#define WALL_JUMP_X (5.5)

#define DASH_FRAMES (18)
#define DASH_FLOAT (0.9)
#define GROUND_POUND_ACCEL (3.0)
#define POUND_FRAMES (60)

#define WALK_SPEED (2.7)
#define JUMP_RELEASE_GRAVITY_FACTOR (1.5)
#define JUMP_HOLD_GRAVITY_FACTOR (0.7)

#define MAX_HEALTH 100

#define INVULN_TIME 30

FLPlayer::FLPlayer() : FLGameObject(32, 64, 14, 32) {
	FLShape *tex_position = new FLShape(x() - 9, y(), 48.f, 32.f);
	shapes.insert(std::make_pair("texture_position", tex_position));
	add_collider("position", "tilemap");
	fl_add_collider_to_group(colliders["tilemap"], "player");
	fl_get_collider(colliders["tilemap"])->add_target_collision_group("items");
	fl_get_collider(colliders["tilemap"])->add_target_collision_group("monsters");
	fl_get_collider(colliders["tilemap"])->add_target_collision_group("projectiles");
	fl_get_collider(colliders["tilemap"])
		->set_collision_method(std::bind(&FLPlayer::handle_collision, this,
										 std::placeholders::_1));

	physics_handler_handle = new_physics_handler(this, "tilemap");

	updator_handle = new_updator(this);

	// Movement and abilities
	dash_frames = 0;
	pound_frames = 0;
	falling_frames = 0;
	invulnerable_frames = 0;

	post_attack_timer = 0;
	attack_held = false;

	cur_ability = FL_NO_ABILITY;
	cur_weapon = FL_FUSION;
	vertical_direction = FL_FORWARD;

	can_use_ability = false;
	can_double_jump = false;
	jump_held = false;
	up_held = false;
	down_held = false;
	right_held = false;
	left_held = false;
	wall_jump_frames = 0;

	// Network
	last_update_tick = SDL_GetTicks();

	// Add to renderer and input mapping
	FLAnimatedObjectParams animation_params = {5, 6, 4, 48.f, 32.f, true};
	FLTexturedObjectParams tex_params = {tex_position, 0, 0, 48.f, 32.f};

	animators["body"] = new FLAnimatedObject(tex_params, animation_params);

	Renderer::getInstance().add_to_world(animators["body"]);

	bind_actions();

	// stats
	max_health = MAX_HEALTH;
	health = max_health;
	target_health = max_health - 50;
	init_weapon_stats();

	chips = 0;
	fragments = 0;
	animation_state = FL_PLAYER_IDLE;

	Renderer::getInstance().get_world_camera()->set_parent(this);
}

FLPlayer::~FLPlayer() {
	Renderer::getInstance().remove_from_world(animators["body"]);
}

void FLPlayer::init_weapon_stats() {
	weapon_stats[FL_FUSION].ammo = 60;
	weapon_stats[FL_FUSION].clip_size = 12;
	weapon_stats[FL_FUSION].recoil = 0.9f;
	weapon_stats[FL_FUSION].posessed = true;
}

void FLPlayer::bind_actions() {
	FLInputHandler& input_handler = FLInputHandler::getInstance();

	// bind actions with player object
	std::function<void(void)> jump = std::bind(&FLPlayer::jump, this);
	std::function<void(void)> interact = std::bind(&FLPlayer::interact, this);
	std::function<void(void)> press_up = std::bind(&FLPlayer::press_up, this);
	std::function<void(void)> press_down = std::bind(&FLPlayer::press_down, this);
	std::function<void(void)> release_up = std::bind(&FLPlayer::release_up, this);
	std::function<void(void)> release_down = std::bind(&FLPlayer::release_down, this);
	std::function<void(void)> hold_jump = std::bind(&FLPlayer::hold_jump, this);
	std::function<void(void)> release_jump =
		std::bind(&FLPlayer::release_jump, this);
	std::function<void(void)> walk_left = std::bind(&FLPlayer::move_left, this);
	std::function<void(void)> walk_right =
		std::bind(&FLPlayer::move_right, this);
	std::function<void(void)> release_left =
		std::bind(&FLPlayer::release_left, this);
	std::function<void(void)> release_right =
		std::bind(&FLPlayer::release_right, this);
	std::function<void(void)> use_ability =
		std::bind(&FLPlayer::use_ability, this);
	std::function<void(void)> attack = std::bind(&FLPlayer::attack, this);
	std::function<void(void)> stop_attack =
		std::bind(&FLPlayer::stop_attack, this);
	std::function<void(void)> drain_ammo =
		std::bind(&FLPlayer::drain_ammo, this);
	std::function<void(void)> animation_update =
		std::bind(&FLPlayer::animation_update, this);

	// map binded actions to input handler
	input_handler.add_game_action(FL_KEY_ACTION2, FL_KEY_PRESSED, jump);
	input_handler.add_game_action(FL_KEY_UP, FL_KEY_RELEASED, release_up);
	input_handler.add_game_action(FL_KEY_DOWN, FL_KEY_RELEASED, release_down);
	input_handler.add_game_action(FL_KEY_UP, FL_KEY_PRESSED, press_up);
	input_handler.add_game_action(FL_KEY_DOWN, FL_KEY_PRESSED, press_down);
	input_handler.add_game_action(FL_KEY_ACTION3, FL_KEY_RELEASED, interact);
	input_handler.add_game_action(FL_KEY_ACTION2, FL_KEY_HELD, hold_jump);
	input_handler.add_game_action(FL_KEY_ACTION2, FL_KEY_RELEASED, release_jump);
	input_handler.add_game_action(FL_KEY_LEFT, FL_KEY_HELD, walk_left);
	input_handler.add_game_action(FL_KEY_RIGHT, FL_KEY_HELD, walk_right);
	input_handler.add_game_action(FL_KEY_LEFT, FL_KEY_RELEASED, release_left);
	input_handler.add_game_action(FL_KEY_RIGHT, FL_KEY_RELEASED, release_right);
	input_handler.add_game_action(FL_KEY_ACTION1, FL_KEY_PRESSED, use_ability);
	input_handler.add_game_action(FL_KEY_ACTION4, FL_KEY_HELD, attack);
	input_handler.add_game_action(FL_KEY_ACTION4, FL_KEY_RELEASED, stop_attack);

	// Other callbacks
	animators["body"]->add_start_callback(drain_ammo);
	animators["body"]->set_animation_update_method(animation_update);
}

// TODO: this should be named differently, it's really the entry point for an attack.
void FLPlayer::drain_ammo() {
	if (attack_held) {
		--weapon_stats[cur_weapon].ammo;
		play_sound("fusion_shoot");
		if (weapon_stats[cur_weapon].ammo <= 0) {
			stop_attack();
		}
		if (facing_right()) {
			physics_handler()->accelerate(-weapon_stats[cur_weapon].recoil, 0);
		} else {
			physics_handler()->accelerate(weapon_stats[cur_weapon].recoil, 0);
		}

		if (vertical_direction == FL_FORWARD) {
			if (facing_right()) {
				new FLFusionProjectile(x() + 8, y() + 10, 11, 0);
			} else {
				new FLFusionProjectile(x() - 18, y() + 10, -11, 0);
			}
		} else {
			if (vertical_direction == FL_UP) {
				new FLFusionProjectile(x() + 8, y() - 2, 0, -11);
			} else {
				new FLFusionProjectile(x() + 8, y() + 20, 0, 11);
			}
		}
	}
}

void FLPlayer::add_ammo(int weapon, int num_clips) {
	weapon_stats[weapon].ammo += num_clips * weapon_stats[weapon].clip_size;
}

void FLPlayer::jump() {
	if (can_wall_jump()) {
		if ((facing_right() && wall_sliding()) || (!facing_right() && !wall_sliding())) {
			physics_handler()->accelerate(-WALL_JUMP_X, WALL_JUMP_Y);
		}
		else {
			physics_handler()->accelerate(WALL_JUMP_X, WALL_JUMP_Y);
		}
	}
	else if (physics_handler()->on_ground()) {
		can_double_jump = true;
		physics_handler()->accelerate(0, JUMP_ACCEL);
		animators["body"]->reset_animation();
		falling_frames = 0;

		// Create a visual smoke effect
		// FIXME need dedicated subclasses of effects etc. with params ready
		// cause this UGLY
		FLTexturedObjectParams tex_params = {nullptr, x(), y() + h() - 16, 32, 16};
		FLAnimatedObjectParams anim_params = {1, 7, 2, 32, 16, false};
		new FLEffect(tex_params, anim_params, 800, 48);

		// play sound effect
		play_sound("player_jump");
	} else if (can_double_jump) {
		FLTexturedObjectParams tex_params = {nullptr, x(), y() + h(), 32, 16};
		FLAnimatedObjectParams anim_params = {1, 7, 2, 32, 16, false};
		new FLEffect(tex_params, anim_params, 800, 32);
		falling_frames = 0;
		double_jump();
		can_double_jump = false;
	}
}

void FLPlayer::use_ability() {
	switch (cur_ability) {
	case FL_DASH:
		dash();
		break;
	default:
		break;
	}
}

void FLPlayer::attack() {
	if (can_attack()) {
		if (!attack_held) {
			animators["body"]->reset_animation();
		}
		attack_held = true;
		post_attack_timer = 20;
	}
}

void FLPlayer::stop_attack() { attack_held = false; }

void FLPlayer::double_jump() {
	physics_handler()->accelerate(0, DOUBLE_JUMP_ACCEL -
										 (0.9 * physics_handler()->yvel()));

	play_sound("player_jump");
	animators["body"]->reset_animation();
}

void FLPlayer::ground_pound() {
	// not implemented atm
}

void FLPlayer::dash() {
	if (can_dash()) {
		play_sound("player_dash");

		// Again, ugly, should have factories or somethin'
		FLTexturedObjectParams tex_params = {nullptr, x(), y(), 16, 32};
		FLAnimatedObjectParams anim_params = {1, 7, 2, 16, 32, false};
		FLEffect *effect = new FLEffect(tex_params, anim_params, 800, 0);

		if (facing_right()) {
			physics_handler()->accelerate(DASH_INITIAL_ACCEL, 0);
			effect->set_reverse(true);
		} else {
			physics_handler()->accelerate(-DASH_INITIAL_ACCEL, 0);
		}

		dash_frames = DASH_FRAMES;
		animators["body"]->reset_animation();
	}
}

bool FLPlayer::pounding() { return pound_frames > 0; }

bool FLPlayer::dashing() { return dash_frames > 0; }

bool FLPlayer::can_dash() { return (!dashing()); }

bool FLPlayer::can_attack() {
	return (cur_weapon != FL_NO_WEAPON && !dashing() &&
			weapon_stats[cur_weapon].ammo > 0 && post_attack_timer < 10);
}

void FLPlayer::move_right() {
	if (physics_handler()->xvel() < WALK_SPEED) {
		// accelerate more if we do not have much momentum, to break past
		// the initial resistance of friction
		if (physics_handler()->xvel() > 0 && physics_handler()->xvel() < 1.5)
			physics_handler()->accelerate(INITIAL_WALK_ACCEL, 0);
		else
			physics_handler()->accelerate(WALK_ACCEL, 0);

		animators["body"]->set_reverse(false);
	}

	right_held = true;
}

void FLPlayer::move_left() {
	if (physics_handler()->xvel() > -WALK_SPEED) {
		// accelerate more if we do not have much momentum, to break past
		// the initial resistance of friction
		if (physics_handler()->xvel() < 0 && physics_handler()->xvel() > -1.5)
			physics_handler()->accelerate(-INITIAL_WALK_ACCEL, 0);
		else
			physics_handler()->accelerate(-WALK_ACCEL, 0);

		animators["body"]->set_reverse(true);
	}

	left_held = true;
}

void FLPlayer::update_health() {
	if (health > target_health) {
		health -= 1;
	} else if (health < target_health) {
		health += 1;
	}
	if (health <= 0) {
		// TODO: death animation
		reset();
	}
}

void FLPlayer::per_frame_update() {
	update_net();
	update_health();

	// This gives the player a bit more control over their jump
	if (jump_held && physics_handler()->yvel() < 0.f) {
		physics_handler()->set_gravity_factor(0.4);
	} else if (!jump_held && physics_handler()->yvel() < 0.f) {
		physics_handler()->set_gravity_factor(1.3);
	} else {
		physics_handler()->set_gravity_factor(1.0);
	}

	if (physics_handler()->on_ground()) {
		hit_ground();
	} else if (++falling_frames > MAX_FALL) {
		reset();
	}

	if (dashing()) {
		--dash_frames;
		if (facing_right()) {
			physics_handler()->accelerate(DASH_ACCEL, 0);
		} else {
			physics_handler()->accelerate(-DASH_ACCEL, 0);
		}
	}

	if (wall_sliding()) {
		wall_jump_frames = 20;
		falling_frames = 0;
	}

	if (wall_jump_frames > 0) {
		wall_jump_frames -= 1;
	}

	if (invulnerable_frames > 0) {
		invulnerable_frames -= 1;
		if (invulnerable_frames % 3 == 0) {
			animators["body"]->set_visible(false);
		} else {
			animators["body"]->set_visible(true);
		}
	} else {
		animators["body"]->set_visible(true);
	}

	if (post_attack_timer > 0) {
		post_attack_timer -= 1;
	}
}

void FLPlayer::animation_update() {
	if (!wall_sliding()) {
		if (facing_right()) {
			shapes["texture_position"]->set_pos(x() - 9, y());
		} else {
			shapes["texture_position"]->set_pos(x() - 25, y());
		}
	}

	// TODO implement animations based on a fsm
	switch (animation_state) {
		case FL_PLAYER_IDLE:
			break;
		case FL_PLAYER_WALK:
			break;
		case FL_PLAYER_JUMP:
			break;
		case FL_PLAYER_FALL:
			break;
		case FL_PLAYER_WALL:
			break;
		case FL_PLAYER_DASH:
			break;
	}

	if (!physics_handler()->on_ground()) {
		if (attacking() && vertical_direction != FL_FORWARD) {
			if (vertical_direction == FL_UP) {
				animators["body"]->set_animation(19);
			} else {
				animators["body"]->set_animation(20);
			}
		}
		else {
			if (physics_handler()->yvel() <= -0.5) {
				if (attacking()) {
					animators["body"]->set_animation(9);
				} else {
					animators["body"]->set_animation(3);
				}
			} else if (falling_frames >= 5) {
				// We check falling frames to avoid jittery animation changes
				if (attacking()) {
					animators["body"]->set_animation(10);
				} else {
					animators["body"]->set_animation(4);
				}
			}
		}
	} else if (running()) {
		if (attacking()) {
			if (vertical_direction == FL_FORWARD) {
				animators["body"]->set_animation(8);
			} else if (vertical_direction == FL_UP) {
				animators["body"]->set_animation(18);
			} else {
				animators["body"]->set_animation(17);
			}
		} else {
			if (vertical_direction == FL_FORWARD) {
				animators["body"]->set_animation(1);
			} else if (vertical_direction == FL_UP) {
				animators["body"]->set_animation(14);
			} else {
				animators["body"]->set_animation(13);
			}
		}
	} else {	// stationary
		if (attacking()) {
			if (vertical_direction == FL_FORWARD) {
				animators["body"]->set_animation(6);
			} else if (vertical_direction == FL_UP) {
				animators["body"]->set_animation(16);
			} else {
				animators["body"]->set_animation(15);
			}
		} else {
			if (vertical_direction == FL_FORWARD) {
				animators["body"]->set_animation(0);
			} else if (vertical_direction == FL_UP) {
				animators["body"]->set_animation(12);
			} else {
				animators["body"]->set_animation(11);
			}
		}
	}
}

void FLPlayer::hold_jump() { jump_held = true; }

void FLPlayer::release_jump() { jump_held = false; }

void FLPlayer::release_left() { left_held = false; }

void FLPlayer::release_right() { right_held = false; }

void FLPlayer::press_up() { 
	up_held = true;
	vertical_direction = FL_UP;
}

void FLPlayer::press_down() {
	down_held = true;
	vertical_direction = FL_DOWN;
}

void FLPlayer::release_up() {
	up_held = false;
	if (down_held) {
		vertical_direction = FL_DOWN;
	} else {
		vertical_direction = FL_FORWARD;
	}
}

void FLPlayer::release_down() {
	down_held = false;
	if (up_held) {
		vertical_direction = FL_UP;
	} else {
		vertical_direction = FL_FORWARD;
	}
}

void FLPlayer::hit_ground() {
	pound_frames = 0;
	falling_frames = 0;
	can_double_jump = true;
}

void FLPlayer::set_reset_position(float x, float y) {
	reset_position.x = x;
	reset_position.y = y;
}

void FLPlayer::reset() {
	falling_frames = 0;
	set_x(reset_position.x);
	set_y(reset_position.y);
	physics_handler()->stop();
	// TODO: update health, ammo etc. based on reset values
	health = max_health;
	target_health = max_health;
	invulnerable_frames = 0;
}

void FLPlayer::set_ability(FLPlayerAbility ability) { cur_ability = ability; }

void FLPlayer::interact() {
	fl_interact(this);
}

void FLPlayer::enable_ability() { can_use_ability = true; }

bool FLPlayer::facing_right() { return (!animators["body"]->reversed()); }

void FLPlayer::update_net() {
	Uint32 tick = SDL_GetTicks();
	int animation = animators["body"]->animation();
	if (animators["body"]->reversed()) {
		animation |= ANIM_REVERSE_BIT;
	}

	update_server_player_info(x(), y(), physics_handler()->xvel(),
							  physics_handler()->yvel(), animation);

	if (tick - last_update_tick >= FL_POS_SEND_INTERVAL) {
		float elapsed_frames = (tick - last_update_tick) / (1000 / 60);
		last_update_tick = tick;
		net_pos.x = x() + (physics_handler()->xvel() * (elapsed_frames));
		// let's just assume y won't really change lol
		net_pos.y = y();
		net_pos.animation = animation;
		send_udp_to_server(FL_MSG_POS, &net_pos, false);
	}
}

float FLPlayer::health_ratio() {
	float ratio = 0.f;
	if (max_health > 0) {
		ratio = float(health) / float(max_health);
	}
	return ratio;
}

float FLPlayer::clip_ratio() {
	int clip_remaining =
		weapon_stats[cur_weapon].ammo % weapon_stats[cur_weapon].clip_size;

	if (clip_remaining == 0 && weapon_stats[cur_weapon].ammo > 0) {
		clip_remaining = weapon_stats[cur_weapon].clip_size;
	}

	return ((float)clip_remaining) /
		   ((float)weapon_stats[cur_weapon].clip_size);
}

void FLPlayer::handle_collision(FLCollider *collision) {
	std::unordered_set<std::string> groups = collision->get_collision_groups();

	if (groups.find("items") != groups.end()) {
		// lol should prob just pass my own collider or something but whatever
		collision->add_collision(nullptr);
	}
	if (groups.find("monsters") != groups.end()) {
		FLMonster* m = (FLMonster*) collision->get_owner();
		point p = m->get_vector_from_player();
		if (p.y >= -4) {
			physics_handler()->accelerate(0, -3);
		} else {
			physics_handler()->accelerate(0, 2);
		}
		if (p.x >= 0) {
			physics_handler()->accelerate(-2, 0);
		} else {
			physics_handler()->accelerate(2, 0);
		}
		hit(10);
	}
	if (groups.find("projectiles") != groups.end()) {
		collision->add_collision(get_collider("tilemap"));
	}
}

void FLPlayer::hit(int damage) {
	if (invulnerable_frames <= 0) {
		target_health = std::max(0, target_health - damage);
		invulnerable_frames = INVULN_TIME;
	}
}

bool FLPlayer::wall_sliding() {
	bool on_wall = physics_handler()->on_wall();
	bool on_ground = physics_handler()->on_ground();

	return (on_wall && (!on_ground));
}

bool FLPlayer::can_wall_jump() {
	return wall_jump_frames > 0;
}

bool FLPlayer::running() {
	return left_held || right_held;
}

void FLPlayer::reset_camera() {
	// TODO: wherever this is called, just call this line instead
	Renderer::getInstance().get_world_camera()->reset();
}

int FLPlayer::get_chips() {
	return chips;
}

int FLPlayer::get_fragments() {
	return fragments;
}

void FLPlayer::add_chip() {
	chips += 1;
}

void FLPlayer::add_fragment() {
	fragments += 1;
}

int FLPlayer::get_ammo() {
	return (weapon_stats[cur_weapon].ammo - 1) / weapon_stats[cur_weapon].clip_size;
}

void FLPlayer::transition_animation(FLPlayerAnimationAction action) {
	// TODO this is unfinished
	switch(action) {
		case FL_PLAYER_JUMP_ACTION:
			if (animation_state != FL_PLAYER_WALL) {
				animation_state = FL_PLAYER_JUMP;
			}
			break;
		case FL_PLAYER_FALL_ACTION:
			if (animation_state != FL_PLAYER_WALL) {
				animation_state = FL_PLAYER_FALL;
			}
			break;
		case FL_PLAYER_ATTACK_ACTION:
			break;
		case FL_PLAYER_RUN_ACTION:
			break;
		case FL_PLAYER_DASH_ACTION:
			break;
		case FL_PLAYER_STOP_ACTION:
			break;
	}
}

bool FLPlayer::attacking() {
	return post_attack_timer > 0;
}

void FLPlayer::heal() {
	target_health = max_health;
}

void FLPlayer::heal(int amount) {
	target_health = target_health + amount;
	if (target_health > max_health) {
		target_health = max_health;
	}
}

