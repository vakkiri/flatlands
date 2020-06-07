/*
 * 	player.cpp
 *
 */

#include <iostream>
#include <stdlib.h>

#include "player.h"

#include "../../audio/fl_audio.h"
#include "../../components/components.h"
#include "../../input/input_handler.h"
#include "../../logging/logging.h"
#include "../../rendering/animated_object.h"
#include "../../rendering/renderer.h"
#include "../../rendering/world_surface.h"
#include "../effect.h"
#include "../physics_settings.h"

#define MAX_FALL (90)
#define INITIAL_WALK_ACCEL (0.39)
#define WALK_ACCEL (0.35)
#define RUN_ACCEL (0.35)
#define DASH_INITIAL_ACCEL (5.0)
#define DASH_ACCEL (0.35)

#define JUMP_ACCEL (-4.3)
#define DOUBLE_JUMP_ACCEL (-4.1)

#define DASH_FRAMES (22)
#define DASH_FLOAT (0.9)
#define GROUND_POUND_ACCEL (3.0)
#define POUND_FRAMES (60)

#define WALK_SPEED (2.2)
#define RUN_SPEED WALK_SPEED // currently we are not supporting running
#define JUMP_RELEASE_GRAVITY_FACTOR (1.5)
#define JUMP_HOLD_GRAVITY_FACTOR (0.8)

#define MAX_HEALTH 100

FLPlayer::FLPlayer() : FLGameObject(32, 64, 16, 32) {
	add_collider("position", "tilemap");
	fl_add_collider_to_group(colliders["tilemap"], "player");
	fl_get_collider(colliders["tilemap"])->add_target_collision_group("items");
	fl_get_collider(colliders["tilemap"])
		->add_target_collision_group("projectiles");
	fl_get_collider(colliders["tilemap"])
		->set_collision_method(std::bind(&FLPlayer::handle_collision, this,
										 std::placeholders::_1));

	physics_handler_handle = new_physics_handler(this, "tilemap");

	updator_handle = new_updator(this);

	// Movement and abilities
	dash_frames = 0;
	pound_frames = 0;
	falling_frames = 0;

	cur_ability = FL_DASH;
	cur_weapon = FL_FUSION;

	can_use_ability = false;
	can_double_jump = false;
	jump_held = false;
	run_held = false;
	attacking = false;

	// Network
	last_update_tick = SDL_GetTicks();

	// Add to renderer and input mapping
	FLAnimatedObjectParams animation_params = {5, 6, 4, 16.f, 32.f, true};
	FLTexturedObjectParams tex_params = {this, 0, 0, 16.f, 32.f};

	FLAnimatedObjectParams wep_animation_params = {1, 6, 2, 16.f, 16.f, true};
	FLTexturedObjectParams wep_tex_params = {this, 0, 10.f, 16.f, 16.f};

	weapon = new FLAnimatedObject(wep_tex_params, wep_animation_params);
	weapon->set_st(0, 224);
	animators["body"] = new FLAnimatedObject(tex_params, animation_params);

	weapon->stop_animation();

	Renderer::getInstance().add_to_world(animators["body"]);
	Renderer::getInstance().add_to_world(weapon);

	bind_actions();

	// stats
	max_health = MAX_HEALTH;
	health = max_health;
	target_health = max_health;
	init_weapon_stats();
}

FLPlayer::~FLPlayer() {
	if (weapon != nullptr)
		delete weapon;
}

void FLPlayer::init_weapon_stats() {
	weapon_stats[FL_FUSION].ammo = 12;
	weapon_stats[FL_FUSION].clip_size = 12;
	weapon_stats[FL_FUSION].recoil = 0.9f;
	weapon_stats[FL_FUSION].posessed = true;
}

void FLPlayer::bind_actions() {
	// bind actions with player object
	std::function<void(void)> jump = std::bind(&FLPlayer::jump, this);
	std::function<void(void)> interact = std::bind(&FLPlayer::interact, this);
	std::function<void(void)> hold_jump = std::bind(&FLPlayer::hold_jump, this);
	std::function<void(void)> release_jump =
		std::bind(&FLPlayer::release_jump, this);
	std::function<void(void)> hold_run = std::bind(&FLPlayer::hold_run, this);
	std::function<void(void)> release_run =
		std::bind(&FLPlayer::release_run, this);
	std::function<void(void)> walk_left = std::bind(&FLPlayer::move_left, this);
	std::function<void(void)> walk_right =
		std::bind(&FLPlayer::move_right, this);
	std::function<void(void)> release_walk =
		std::bind(&FLPlayer::release_walk, this);
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
	FLInputHandler::getInstance().add_game_action(FL_KEY_ACTION3, FL_KEY_HELD,
												  hold_run);
	FLInputHandler::getInstance().add_game_action(FL_KEY_ACTION3,
												  FL_KEY_RELEASED, release_run);
	FLInputHandler::getInstance().add_game_action(FL_KEY_ACTION2,
												  FL_KEY_PRESSED, jump);
	FLInputHandler::getInstance().add_game_action(FL_KEY_DOWN, FL_KEY_PRESSED,
												  interact);
	FLInputHandler::getInstance().add_game_action(FL_KEY_ACTION2, FL_KEY_HELD,
												  hold_jump);
	FLInputHandler::getInstance().add_game_action(
		FL_KEY_ACTION2, FL_KEY_RELEASED, release_jump);
	FLInputHandler::getInstance().add_game_action(FL_KEY_LEFT, FL_KEY_HELD,
												  walk_left);
	FLInputHandler::getInstance().add_game_action(FL_KEY_RIGHT, FL_KEY_HELD,
												  walk_right);
	FLInputHandler::getInstance().add_game_action(FL_KEY_RIGHT, FL_KEY_RELEASED,
												  release_walk);
	FLInputHandler::getInstance().add_game_action(FL_KEY_LEFT, FL_KEY_RELEASED,
												  release_walk);
	FLInputHandler::getInstance().add_game_action(FL_KEY_ACTION1,
												  FL_KEY_PRESSED, use_ability);
	FLInputHandler::getInstance().add_game_action(FL_KEY_ACTION4, FL_KEY_HELD,
												  attack);
	FLInputHandler::getInstance().add_game_action(FL_KEY_ACTION4,
												  FL_KEY_RELEASED, stop_attack);

	// Other callbacks
	weapon->add_start_callback(drain_ammo);
	animators["body"]->set_animation_update_method(animation_update);
}

void FLPlayer::drain_ammo() {
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
}

void FLPlayer::add_ammo(int weapon, int num_clips) {
	weapon_stats[weapon].ammo += num_clips * weapon_stats[weapon].clip_size;
}

void FLPlayer::jump() {
	if (physics_handler()->on_ground()) {
		can_double_jump = true;
		physics_handler()->accelerate(0, JUMP_ACCEL);
		animators["body"]->reset_animation();
		falling_frames = 0;

		// Create a visual smoke effect
		// FIXME need dedicated subclasses of effects etc. with params ready
		// cause this UGLY
		FLTexturedObjectParams tex_params = {nullptr, x() - (w() / 2.f),
											 y() + h() - 16, 32, 16};
		FLAnimatedObjectParams anim_params = {1, 7, 2, 32, 16, false};
		new FLEffect(tex_params, anim_params, 288, 48);

		// play sound effect
		play_sound("player_jump");
	} else if (can_double_jump) {
		FLTexturedObjectParams tex_params = {nullptr, x() - (w() / 2.f),
											 y() + h(), 32, 16};
		FLAnimatedObjectParams anim_params = {1, 7, 2, 32, 16, false};
		new FLEffect(tex_params, anim_params, 288, 32);
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
		attacking = true;
	}
}

void FLPlayer::stop_attack() { attacking = false; }

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
		FLEffect *effect = new FLEffect(tex_params, anim_params, 288, 0);

		dash_right = facing_right();
		if (dash_right) {
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
	return (cur_weapon != FL_NO_WEAPON && state != FL_PLAYER_DASH &&
			weapon_stats[cur_weapon].ammo > 0);
}

void FLPlayer::move_right() {
	if (physics_handler()->xvel() < WALK_SPEED ||
		(run_held && physics_handler()->xvel() < RUN_SPEED)) {
		// accelerate more if we do not have much momentum, to break past
		// the initial resistance of friction
		if (physics_handler()->xvel() > 0 && physics_handler()->xvel() < 1.5)
			physics_handler()->accelerate(INITIAL_WALK_ACCEL, 0);
		else if (run_held)
			physics_handler()->accelerate(RUN_ACCEL, 0);
		else
			physics_handler()->accelerate(WALK_ACCEL, 0);

		animators["body"]->set_reverse(false);
	}

	if (state != FL_PLAYER_DASH)
		state = FL_PLAYER_WALK;
}

void FLPlayer::move_left() {
	if (physics_handler()->xvel() > -WALK_SPEED ||
		(run_held && physics_handler()->xvel() > -RUN_SPEED)) {
		// accelerate more if we do not have much momentum, to break past
		// the initial resistance of friction
		if (physics_handler()->xvel() < 0 && physics_handler()->xvel() > -1.5)
			physics_handler()->accelerate(-INITIAL_WALK_ACCEL, 0);
		else if (run_held)
			physics_handler()->accelerate(-RUN_ACCEL, 0);
		else
			physics_handler()->accelerate(-WALK_ACCEL, 0);

		animators["body"]->set_reverse(true);
	}

	if (state != FL_PLAYER_DASH)
		state = FL_PLAYER_WALK;
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
	update_camera();
	update_health();

	// This gives the player a bit more control over their jump
	if (jump_held && physics_handler()->yvel() < 0.f) {
		physics_handler()->set_gravity_factor(0.5);
	} else if (!jump_held && physics_handler()->yvel() < 0.f) {
		physics_handler()->set_gravity_factor(2.5);
	} else {
		physics_handler()->set_gravity_factor(1.0);
	}

	if (physics_handler()->on_ground()) {
		hit_ground();
	} else {
		state = FL_PLAYER_JUMP;

		if (++falling_frames > MAX_FALL) {
			reset();
		}
	}

	if (dashing()) {
		--dash_frames;
		if (facing_right()) {
			physics_handler()->accelerate(DASH_ACCEL, 0);
		} else {
			physics_handler()->accelerate(-DASH_ACCEL, 0);
		}
		state = FL_PLAYER_DASH;
	}
}

void FLPlayer::update_camera() {
	Renderer &r = Renderer::getInstance();
	float xamt = 0;
	float yamt = 0;
	float xoffset;
	if (animators["body"]->reversed())
		xoffset = 32;
	else
		xoffset = 64;

	float yoffset = -16;

	float dx = (r.world_camera_x() / 2) + x() + xoffset;
	float dy = (r.world_camera_y() / 2) + y() + yoffset;

	// TODO: replace hardcoded values with screen width/height * camera scale
	xamt = (dx / 50) * -5.0;
	yamt = (dy / 50) * -5.0;

	r.translate_world_camera(glm::vec3(xamt, yamt, 0));
}

void FLPlayer::animation_update() {
	/* set weapon position to our own */
	if (!animators["body"]->reversed()) {
		weapon->set_x(6.f);
	} else {
		weapon->set_x(-6.f);
	}
	weapon->set_y(14.f);
	weapon->set_reverse(animators["body"]->reversed());

	switch (cur_weapon) {
	case FL_NO_WEAPON:
		break;
	case FL_FUSION:
		weapon->set_st(0, 224);
		weapon->set_steps(16, 0);
		weapon->set_repeats(true);
		break;
	default:
		break;
	}

	switch (state) {
	case FL_PLAYER_IDLE:
		if (attacking) {
			animators["body"]->set_animation(6);
		} else {
			animators["body"]->set_animation(0);
		}
		break;
	case FL_PLAYER_WALK:
		if (attacking) {
			animators["body"]->set_animation(2);
		} else {
			animators["body"]->set_animation(1);
		}
		break;
	case FL_PLAYER_JUMP:
		if (attacking) {
			animators["body"]->set_animation(4);
		} else {
			animators["body"]->set_animation(3);
		}
		break;
	case FL_PLAYER_DASH:
		animators["body"]->set_animation(5);
		attacking = false;
		break;
	default:
		break;
	}

	if (attacking) {
		weapon->start_animation();
		weapon->set_visible(true);
	} else {
		weapon->set_repeats(false);
		weapon->set_visible(false);

		if (weapon->finished()) {
			weapon->reset_animation();
			weapon->stop_animation();
		}
	}
	if (state == FL_PLAYER_WALK && physics_handler()->on_ground()) {
		start_sound("player_walk");
	}
}

void FLPlayer::hold_jump() { jump_held = true; }

void FLPlayer::release_jump() { 
	jump_held = false; 
}

void FLPlayer::hold_run() { run_held = true; }

void FLPlayer::release_run() { run_held = false; }

void FLPlayer::release_walk() {
	if (!dashing())
		state = FL_PLAYER_IDLE;

	stop_sound("player_walk");
}

void FLPlayer::hit_ground() {
	if (state != FL_PLAYER_WALK && !dashing())
		state = FL_PLAYER_IDLE;

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
}

void FLPlayer::set_ability(FLPlayerAbility ability) { cur_ability = ability; }

void FLPlayer::interact() {
	// stub
}

void FLPlayer::enable_ability() { can_use_ability = true; }

bool FLPlayer::facing_right() { return (!animators["body"]->reversed()); }

FLAnimatedObject *FLPlayer::get_weapon() { return weapon; }

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
	}
	if (groups.find("projectiles") != groups.end()) {
		collision->add_collision(nullptr);
	}
}

void FLPlayer::hit(int damage) {
	target_health = std::max(0, target_health - damage);
}
