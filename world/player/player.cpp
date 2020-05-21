/*
 * 	player.cpp
 *
 */

#include <iostream>
#include <stdlib.h>

#include "player.h"

#include "../effect.h"
#include "../physics_settings.h"
#include "../../audio/fl_audio.h"
#include "../../components/components.h"
#include "../../input/input_handler.h"
#include "../../rendering/world_surface.h"
#include "../../rendering/renderer.h"
#include "../../logging/logging.h"

#define MAX_FALL 		(90)
#define INITIAL_WALK_ACCEL 	(0.39)
#define WALK_ACCEL 		(0.35)
#define RUN_ACCEL 		(0.35)
#define DASH_INITIAL_ACCEL 	(5.0)
#define DASH_ACCEL 		(0.35)

#define JUMP_ACCEL 		(-4.3)
#define DOUBLE_JUMP_ACCEL 	(-4.1)

#define DASH_FRAMES 		(22)
#define DASH_FLOAT		(0.95)
#define GROUND_POUND_ACCEL 	(3.0)
#define POUND_FRAMES 		(60)

#define WALK_SPEED 			(2.2)
#define RUN_SPEED 			WALK_SPEED	// currently we are not supporting running
#define JUMP_RELEASE_GRAVITY_FACTOR 	(1.5)
#define JUMP_HOLD_GRAVITY_FACTOR 	(0.8)

#define MAX_HEALTH	100

FLPlayer::FLPlayer() : FLGameObject( 32, 64, 16, 32 ), FLAnimatedObject( 5, 6, 4, 16.f, 32.f ) {
	add_collider( "position", "tilemap" );
	physics_handler_handle = new_physics_handler( this, "tilemap" );
	updator_handle = new_updator( this );

	// Movement and abilities
	dash_frames = 		0;
	pound_frames = 		0;
	falling_frames = 	0;

	cur_ability = 		FL_DASH;
	cur_weapon = 		FL_FUSION;

	can_use_ability = 	false;
	can_double_jump = 	false;
	jump_held = 		false;
	run_held = 		false;
	attacking =		false;

	// Network
	last_update_tick = SDL_GetTicks();

	// Weapon
	weapon = new FLAnimatedObject( 1, 6, 5, 16.f, 16.f );
	weapon->set_repeats(false);
	weapon->stop_animation();
	init_weapon_stats();

	// stats
	max_health = MAX_HEALTH;
	health = max_health;

	// Add to renderer and input mapping
	Renderer::getInstance().add_to_world(this);
	Renderer::getInstance().add_to_world(weapon);

	bind_actions();
}

FLPlayer::~FLPlayer() {
	if (weapon != nullptr)
		delete weapon;
}

void FLPlayer::init_weapon_stats() {
	for (int i = 0; i < FL_NUM_WEAPONS; ++i) {
		memset(&(weapon_stats[i]), 0, sizeof(weapon_stats));
	}

	weapon_stats[FL_FUSION].ammo = 12;
	weapon_stats[FL_FUSION].clip_size = 12;
	weapon_stats[FL_FUSION].recoil = 0.9f;
}

void FLPlayer::bind_actions() {
	// bind actions with player object
	std::function<void(void)> jump = std::bind(&FLPlayer::jump, this);
	std::function<void(void)> interact = std::bind(&FLPlayer::interact, this);
	std::function<void(void)> hold_jump = std::bind(&FLPlayer::hold_jump, this);
	std::function<void(void)> release_jump = std::bind(&FLPlayer::release_jump, this);
	std::function<void(void)> hold_run = std::bind(&FLPlayer::hold_run, this);
	std::function<void(void)> release_run = std::bind(&FLPlayer::release_run, this);
	std::function<void(void)> walk_left = std::bind(&FLPlayer::move_left, this);
	std::function<void(void)> walk_right = std::bind(&FLPlayer::move_right, this);
	std::function<void(void)> release_walk = std::bind(&FLPlayer::release_walk, this);
	std::function<void(void)> use_ability = std::bind(&FLPlayer::use_ability, this);
	std::function<void(void)> attack = std::bind(&FLPlayer::attack, this);
	std::function<void(void)> stop_attack = std::bind(&FLPlayer::stop_attack, this);
	std::function<void(void)> drain_ammo = std::bind(&FLPlayer::drain_ammo, this);

	// map binded actions to input handler
	FLInputHandler::getInstance().add_game_action(FL_KEY_ACTION3, FL_KEY_HELD, hold_run);
	FLInputHandler::getInstance().add_game_action(FL_KEY_ACTION3, FL_KEY_RELEASED, release_run);
	FLInputHandler::getInstance().add_game_action(FL_KEY_ACTION2, FL_KEY_PRESSED, jump);
	FLInputHandler::getInstance().add_game_action(FL_KEY_DOWN, FL_KEY_PRESSED, interact);
	FLInputHandler::getInstance().add_game_action(FL_KEY_ACTION2, FL_KEY_HELD, hold_jump);
	FLInputHandler::getInstance().add_game_action(FL_KEY_ACTION2, FL_KEY_RELEASED, release_jump);
	FLInputHandler::getInstance().add_game_action(FL_KEY_LEFT, FL_KEY_HELD, walk_left);
	FLInputHandler::getInstance().add_game_action(FL_KEY_RIGHT, FL_KEY_HELD, walk_right);
	FLInputHandler::getInstance().add_game_action(FL_KEY_RIGHT, FL_KEY_RELEASED, release_walk);
	FLInputHandler::getInstance().add_game_action(FL_KEY_LEFT, FL_KEY_RELEASED, release_walk);
	FLInputHandler::getInstance().add_game_action(FL_KEY_ACTION1, FL_KEY_PRESSED, use_ability);
	FLInputHandler::getInstance().add_game_action(FL_KEY_ACTION4, FL_KEY_HELD, attack);
	FLInputHandler::getInstance().add_game_action(FL_KEY_ACTION4, FL_KEY_RELEASED, stop_attack);

	// Other callbacks
	weapon->add_start_callback(drain_ammo);
}

void FLPlayer::drain_ammo() {
	--weapon_stats[cur_weapon].ammo;
	play_sound("fusion_shoot");
	if ( weapon_stats[cur_weapon].ammo <= 0 ) {
		stop_attack();
	}
	if (facing_right()) {
		physics_handler()->accelerate( -weapon_stats[cur_weapon].recoil, 0 );
	}
	else {
		physics_handler()->accelerate( weapon_stats[cur_weapon].recoil, 0 );
	}
}

void FLPlayer::add_ammo( int weapon, int num_clips ) {
	weapon_stats[weapon].ammo += num_clips * weapon_stats[weapon].clip_size;
}

void FLPlayer::jump() {
	if ( physics_handler()->on_ground() && !can_double_jump ) {
		can_double_jump = true;
		physics_handler()->accelerate( 0, JUMP_ACCEL );
		reset_animation();
		falling_frames = 0;

		// Create a visual smoke effect
		new FLEffect( x() - (w()/2.f), y() + h() - 16, 288, 48, 7, 32, 16 );
		
		// play sound effect
		play_sound( "player_jump" );
	}
	else if ( can_double_jump ) {
		new FLEffect( x() - (w()/2.f), y() + h() - 16, 288, 32, 7, 32, 16 );
		falling_frames = 0;
		double_jump();
		can_double_jump = false;
	}
}


void FLPlayer::use_ability() {
	switch ( cur_ability ) {
		case FL_DASH:
			dash();
			break;
		default:
			break;
	}
}

void FLPlayer::attack() {
	if ( can_attack() ) {
		attacking = true;
	}
}

void FLPlayer::stop_attack() {
	attacking = false;
}

void FLPlayer::double_jump() {
	physics_handler()->accelerate( 0, DOUBLE_JUMP_ACCEL - (0.9 * physics_handler()->yvel()) );

	play_sound( "player_jump" );
	reset_animation();
}

void FLPlayer::ground_pound() {
	// not implemented atm
}

void FLPlayer::dash() {
	if ( can_dash() ) {
		play_sound( "player_dash" );
		FLEffect* effect = new FLEffect( x(), y(), 288, 0, 8, 16, 32 );
		dash_right = facing_right();
		if (dash_right) {
			physics_handler()->accelerate( DASH_INITIAL_ACCEL, 0 );
			effect->set_reverse(true);
		}
		else {
			physics_handler()->accelerate( -DASH_INITIAL_ACCEL, 0 );
		}

		dash_frames = DASH_FRAMES;
		reset_animation();
	}
}

bool FLPlayer::pounding() {
	return pound_frames > 0;
}

bool FLPlayer::dashing() {
	return dash_frames > 0;
}

bool FLPlayer::can_dash() {
	return (!dashing());
}

bool FLPlayer::can_attack() {
	return (	cur_weapon != FL_NO_WEAPON &&
			state != FL_PLAYER_DASH &&
			weapon_stats[cur_weapon].ammo > 0
			);
}

void FLPlayer::move_right() {
	if ( physics_handler()->xvel() < WALK_SPEED || (run_held && physics_handler()->xvel() < RUN_SPEED) ) {
		// accelerate more if we do not have much momentum, to break past
		// the initial resistance of friction
		if ( physics_handler()->xvel() > 0 && physics_handler()->xvel() < 1.5 )
			physics_handler()->accelerate(INITIAL_WALK_ACCEL, 0);
		else if ( run_held )
			physics_handler()->accelerate(RUN_ACCEL, 0);
		else
			physics_handler()->accelerate(WALK_ACCEL, 0);

		set_reverse(false);
		 
	}

	if ( state != FL_PLAYER_DASH )
		state = FL_PLAYER_WALK;
}

void FLPlayer::move_left() {
	if ( physics_handler()->xvel() > -WALK_SPEED || (run_held && physics_handler()->xvel() > -RUN_SPEED) ) {
		// accelerate more if we do not have much momentum, to break past
		// the initial resistance of friction
		if ( physics_handler()->xvel() < 0 && physics_handler()->xvel() > -1.5 )
			physics_handler()->accelerate(-INITIAL_WALK_ACCEL, 0);
		else if ( run_held )
			physics_handler()->accelerate(-RUN_ACCEL, 0);
		else
			physics_handler()->accelerate(-WALK_ACCEL, 0);

		set_reverse(true);
	}

	if ( state != FL_PLAYER_DASH )
		state = FL_PLAYER_WALK;
}

void FLPlayer::per_frame_update() {
	update_net();

	// This gives the player a bit more control over their jump
	if ( jump_held && physics_handler()->yvel() < 0.f ) {
		physics_handler()->set_gravity_factor( 0.5 );
	}
	else {
		physics_handler()->set_gravity_factor( 1.0 );
	}

	if ( physics_handler()->on_ground() ) {
		hit_ground();
	}
	else {
		state = FL_PLAYER_JUMP;

		if ( ++falling_frames > MAX_FALL ) {
			reset();
		}
	}

	if ( dashing() ) {
		--dash_frames;
		if ( facing_right() ) { 
			physics_handler()->accelerate( DASH_ACCEL, 0 );
		}
		else {
			physics_handler()->accelerate( -DASH_ACCEL, 0 );
		}
		state = FL_PLAYER_DASH;
	}
}

void FLPlayer::update_camera() {
	Renderer& r = Renderer::getInstance();
	float xamt = 0;
	float yamt = 0;
	float xoffset;
	if ( reverse )
		xoffset = 32;
	else
		xoffset = 64;

	float yoffset = -16;

	float dx = (r.world_camera_x() / 2) + x() + xoffset;
	float dy = (r.world_camera_y() / 2) + y() + yoffset;

	// TODO: replace hardcoded values with screen width/height * camera scale
	xamt = (dx / 50) * -5.0;
	yamt = (dy / 50) * -5.0;

	r.translate_world_camera( glm::vec3( xamt, yamt, 0 ) );
}

void FLPlayer::update_animation() {
	/* set weapon position to our own */
	if (!reverse) {
		weapon->set_x( x() + 6.f);
	}
	else {
		weapon->set_x( x() - 6.f);
	}
	weapon->set_y( y() + 14.f );
	weapon->set_reverse( reverse );

	switch ( cur_weapon ) {
		case FL_NO_WEAPON:
			break;
		case FL_FUSION:
			weapon->set_st(96, 0);
			weapon->set_steps(16, 0);
			//weapon->set_w(16.f);
			//weapon->set_h(16.f);
			weapon->set_repeats(true);
			break;
		default:
			break;
	}
	
	switch ( state ) {
		case FL_PLAYER_IDLE:
			if ( attacking ) {
				set_animation( 6 );
			}
			else {
				set_animation( 0 );
			}
			break;
		case FL_PLAYER_WALK:
			if ( attacking ) {
				set_animation( 2 );
			}
			else {
				set_animation( 1 );
			}
			break;
		case FL_PLAYER_JUMP:
			if ( attacking ) {
				set_animation( 4 );
			}
			else {
				set_animation( 3 );
			}
			break;
		case FL_PLAYER_DASH:
			set_animation( 5 );
			attacking = false;
			break;
		default:
			break;
	}

	if ( attacking ) {
		weapon->start_animation();
		weapon->set_visible(true);
	}
	else {
		weapon->set_repeats(false);
		weapon->set_visible(false);

		if (weapon->finished()) {
			weapon->reset_animation();
			weapon->stop_animation();
		}
	}
	if ( state == FL_PLAYER_WALK && physics_handler()->on_ground() ) {
		start_sound("player_walk");
	}
	
	weapon->update_animation();
	FLAnimatedObject::update_animation();
	update_camera();
}

void FLPlayer::hold_jump() { jump_held = true; }

void FLPlayer::release_jump() { 
	jump_held = false; 
}

void FLPlayer::hold_run() { run_held = true; }

void FLPlayer::release_run() { run_held = false; }

void FLPlayer::release_walk() { 
	if ( !dashing() )
		state = FL_PLAYER_IDLE; 

	stop_sound("player_walk");
}

void FLPlayer::hit_ground() {
	if ( state != FL_PLAYER_WALK && !dashing() )
		state = FL_PLAYER_IDLE;

	pound_frames = 0;
	falling_frames = 0;
	can_double_jump = true;
}

void FLPlayer::set_reset_position( float x, float y ) {
	reset_position.x = x;
	reset_position.y = y;
}

void FLPlayer::reset() {
	falling_frames = 0;
	set_x( reset_position.x );
	set_y( reset_position.y );
	physics_handler()->stop();
	// TODO: update health, ammo etc. based on reset values
	health = max_health;
}

void FLPlayer::set_ability( FLPlayerAbility ability ) {
	cur_ability = ability;
}

void FLPlayer::interact() {
	// stub
}

void FLPlayer::enable_ability() {
	can_use_ability = true;
}

bool FLPlayer::facing_right() {
	return (!reverse);
}

FLAnimatedObject* FLPlayer::get_weapon() {
	return weapon;
}

void FLPlayer::update_net() {
	Uint32 tick = SDL_GetTicks();
	int animation = cur_animation;
	if (reverse) {
		animation |= ANIM_REVERSE_BIT;
	}

	update_server_player_info( x(), y(), physics_handler()->xvel(), physics_handler()->yvel(), animation );

	if ( tick - last_update_tick >= FL_POS_SEND_INTERVAL ) {
		float elapsed_frames = (tick - last_update_tick) / (1000/60);
		last_update_tick = tick;
		net_pos.x = x() + (physics_handler()->xvel() * (elapsed_frames));
		// let's just assume y won't really change lol
		net_pos.y = y();
		net_pos.animation = animation;
		send_udp_to_server( FL_MSG_POS, &net_pos, false );
	}
}

float FLPlayer::health_ratio() {
	return health / max_health;
}

float FLPlayer::clip_ratio() {
	int clip_remaining = weapon_stats[cur_weapon].ammo % weapon_stats[cur_weapon].clip_size;

	if ( clip_remaining == 0 && weapon_stats[cur_weapon].ammo > 0 ) {
		clip_remaining = weapon_stats[cur_weapon].clip_size;
	}

	return ((float) clip_remaining) / ((float) weapon_stats[cur_weapon].clip_size);
}

