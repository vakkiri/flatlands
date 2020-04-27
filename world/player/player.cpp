/*
 * 	player.cpp
 *
 */

#include <iostream>
#include <stdlib.h>

#include "player.h"

#include "../colliding_object.h"
#include "../effect.h"
#include "../physics_settings.h"
#include "../world_environment.h"
#include "../../audio/fl_audio.h"
#include "../../input/input_handler.h"
#include "../../rendering/world_surface.h"
#include "../../rendering/renderer.h"
#include "../../logging/logging.h"

#define MAX_FALL 		(90)
#define INITIAL_WALK_ACCEL 	(0.54)
#define WALK_ACCEL 		(0.49)
#define RUN_ACCEL 		(0.49)
#define DASH_INITIAL_ACCEL 	(4.3)
#define DASH_ACCEL 		(0.4)

#define INITIAL_JUMP_VEL 	(-4.1)
#define JUMP_FRAME_ACCEL 	(0.2)
#define NUM_JUMP_FRAMES 	(30)

#define DASH_FRAMES 		(25)
#define DASH_FLOAT		(0.95)
#define DOUBLE_JUMP_ACCEL 	(3.6)
#define GROUND_POUND_ACCEL 	(3.0)
#define POUND_FRAMES 		(60)

#define WALK_SPEED 			(2.5)
#define RUN_SPEED 			(2.5)
#define X_TERMINAL_VELOCITY 		(5.2)
#define Y_TERMINAL_VELOCITY 		(6.1)
#define JUMP_RELEASE_GRAVITY_FACTOR 	(1.5)
#define JUMP_HOLD_GRAVITY_FACTOR 	(0.8)

FLPlayer::FLPlayer() : FLAnimatedObject( 5, 6, 4, 16.f, 32.f ) {

	// Size and bounds
	position.x = 32;
	position.y = 64;
	position.w = 16;
	position.h = 32;
	rect bounds;
	bounds.x = 2;
	bounds.y = 10;
	bounds.w = -4;
	bounds.h = -10;

	set_bounds_margin(bounds);

	// Animation
	set_start_repeat(10, 0);

	// Movement and abilities
	jump_frames = 		0;
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

	// Weapon
	weapon = new FLAnimatedObject( 1, 4, 4, 16.f, 16.f );
	weapon->set_repeats(false);
	weapon->stop_animation();
	init_weapon_stats();

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

	weapon_stats[FL_FUSION].ammo = 10;
	weapon_stats[FL_FUSION].recoil = 2.f;
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
	if ( weapon_stats[cur_weapon].ammo <= 0 ) {
		stop_attack();
	}
}

void FLPlayer::add_ammo( int weapon, int amount ) {
	weapon_stats[weapon].ammo += amount;
}

void FLPlayer::jump() {
	if ( on_ground() ) {
		can_double_jump = true;
		vel.y = INITIAL_JUMP_VEL;
		reset_animation();
		on_ground_timer = 0;
		falling_frames = 0;
		jump_frames = NUM_JUMP_FRAMES;

		// Create a visual smoke effect
		new FLEffect( x() - (w()/2.f), y() + h() - 16, 0, 496, 7, 32, 16 );
		
		// play sound effect
		play_sound( "player_jump" );
	}
	else if ( can_double_jump ) {
		new FLEffect( x() - (w()/2.f), y() + h() - 16, 0, 480, 9, 32, 16 );
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
	vel.y = -DOUBLE_JUMP_ACCEL;

	play_sound( "player_jump" );
	reset_animation();
}

void FLPlayer::ground_pound() {
	vel.y = GROUND_POUND_ACCEL;
	pound_frames = POUND_FRAMES;

	// create pound effect
	new FLEffect( x(), y(), 64, 0, 6, 16, 16 );
}

void FLPlayer::dash() {
	if ( can_dash() ) {
		play_sound( "player_dash" );
		FLEffect* effect = new FLEffect( x(), y(), 0, 448, 8, 16, 32 );
		dash_right = facing_right();
		if (dash_right) {
			vel.x += DASH_INITIAL_ACCEL;
			effect->set_reverse(true);
		}
		else {
			vel.x -= DASH_INITIAL_ACCEL;
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
	if ( vel.x < WALK_SPEED || (run_held && vel.x < RUN_SPEED) ) {
		// accelerate more if we do not have much momentum, to break past
		// the initial resistance of friction
		if ( vel.x > 0 && vel.x < 1.5 )
			accelerate(point(INITIAL_WALK_ACCEL, 0));
		else if ( run_held )
			accelerate(point(RUN_ACCEL, 0));
		else
			accelerate(point(WALK_ACCEL, 0));

		set_reverse(false);
		 
	}

	if ( state != FL_PLAYER_DASH )
		state = FL_PLAYER_WALK;
}

void FLPlayer::move_left() {
	if ( vel.x > -WALK_SPEED || (run_held && vel.x > -RUN_SPEED) ) {
		// accelerate more if we do not have much momentum, to break past
		// the initial resistance of friction
		if ( vel.x < 0 && vel.x > -1.5 )
			accelerate(point(-INITIAL_WALK_ACCEL, 0));
		else if ( run_held )
			accelerate(point(-RUN_ACCEL, 0));
		else
			accelerate(point(-WALK_ACCEL, 0));

		set_reverse(true);
	}

	if ( state != FL_PLAYER_DASH )
		state = FL_PLAYER_WALK;
}

void FLPlayer::bound_velocity() { 
	// TODO: this should probably be moved to a more specific update function
	if ( dashing() ) {
		if ( dash_right ) {
			vel.x += DASH_ACCEL;
		}
		else {
			vel.x -= DASH_ACCEL;
		}
		if ( vel.y > 0 ) {
			vel.y *= DASH_FLOAT;
		}

		--dash_frames;
	}
	else if ( state == FL_PLAYER_DASH ) {
		state = FL_PLAYER_IDLE;
	}

	if ( pound_frames > 0 )
		pound_frames--;
	else {
		if ( vel.x > X_TERMINAL_VELOCITY )
			vel.x = X_TERMINAL_VELOCITY;
		else if ( vel.x < -X_TERMINAL_VELOCITY )
			vel.x = -X_TERMINAL_VELOCITY;
	}

	if ( vel.y > Y_TERMINAL_VELOCITY )
		vel.y = Y_TERMINAL_VELOCITY;
	else if ( vel.y < -Y_TERMINAL_VELOCITY )
		vel.y = -Y_TERMINAL_VELOCITY;
}

void FLPlayer::update_physics() {
	FLPhysicsObject::update_physics();

	// reduce effect of gravity immediately after jumping
	if ( jump_frames > 0 ) {
		accel.y = -JUMP_FRAME_ACCEL;
		jump_frames--;
	}

	if ( on_ground() ) {
		hit_ground();
	}
	else {
		state = FL_PLAYER_JUMP;

		if ( ++falling_frames > MAX_FALL )
			reset();
	}

	// abilities take precedence for state
	if ( pounding() ) {
		state = FL_PLAYER_POUND;
		if ( pound_frames % 5 == 0 )
			new FLEffect( x(), y(), 64, 0, 6, 16, 16 );
	}
	if ( dashing() ) {
		state = FL_PLAYER_DASH;
	}
	

	bound_velocity();

	update_position();

	std::vector<FLCollidingObject*> collisions = FLWorldEnvironment::getInstance().find_colliding_objects( this );

	for ( FLCollidingObject* collision : collisions )
		collision->collide_with( this );
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
	xamt = (dx / 50) * -X_TERMINAL_VELOCITY;
	yamt = (dy / 50) * -5.0;

	r.translate_world_camera( glm::vec3( xamt, yamt, 0 ) );
}

void FLPlayer::update_animation() {
	/* set weapon position to our own */
	if (!reverse) {
		weapon->set_x( x() + 8.f);
	}
	else {
		weapon->set_x( x() - 8.f);
	}
	weapon->set_y( y() + 16.f );
	weapon->set_reverse( reverse );
	/* the weapon switch comes first, since the later state switch may override weapon visibility */
	switch ( cur_weapon ) {
		case FL_NO_WEAPON:
			weapon->set_visible( false );
			break;
		case FL_FUSION:
			weapon->set_st(96, 0);
			weapon->set_steps(16, 0);
			weapon->set_w(16.f);
			weapon->set_h(16.f);
			weapon->set_repeats(true);
			weapon->set_visible(true);
			break;
		default:
			weapon->set_visible(false);
			break;
	}
	if ( attacking ) {
		weapon->start_animation();
		start_sound("fusion_shoot");
	}
	else {
		stop_sound("fusion_shoot");
		weapon->set_visible(false);
		weapon->set_repeats(false);

		if (weapon->finished()) {
			weapon->reset_animation();
			weapon->stop_animation();
		}
	}
	switch ( state ) {
		case FL_PLAYER_IDLE:
			set_animation( 0 );
			break;
		case FL_PLAYER_WALK:
			set_animation( 1 );
			break;
		case FL_PLAYER_JUMP:
			set_animation( 2 );
			break;
		case FL_PLAYER_DASH:
			set_animation( 3 );
			weapon->set_visible(false);
			attacking = false;
			break;
		default:
			break;
	}

	if ( state == FL_PLAYER_WALK && on_ground() ) {
		start_sound("player_walk");
	}
	
	weapon->update_animation();
	FLAnimatedObject::update_animation();
	update_camera();
}

void FLPlayer::apply_gravity() { 
	/*
	 *	Gravity tweaks:
	 *
	 *	- before the jump peak, less gravity is applied if holding jump
	 *	- after the peak, extra gravity is applied if not holding jump
	 *
	 */
	if ( !jump_held && vel.y > 0 )
		vel.y += physics.gravity() * JUMP_RELEASE_GRAVITY_FACTOR;
	else if ( jump_held && vel.y < 0 )
		vel.y += physics.gravity() * JUMP_HOLD_GRAVITY_FACTOR;
	else
		vel.y += physics.gravity();
}

void FLPlayer::hold_jump() { jump_held = true; }

void FLPlayer::release_jump() { 
	jump_held = false; 
}

void FLPlayer::hold_run() { run_held = true; }

void FLPlayer::release_run() { run_held = false; }

void FLPlayer::release_walk() { 
	if ( state != FL_PLAYER_DASH )
		state = FL_PLAYER_IDLE; 

	stop_sound("player_walk");
}

void FLPlayer::hit_ground() {
	if ( state != FL_PLAYER_WALK && state != FL_PLAYER_DASH )
		state = FL_PLAYER_IDLE;

	if ( pounding() ) {
		new FLEffect( x() - 12, y(), 64, 0, 6, 16, 16 );
		new FLEffect( x() - 20, y() + 8, 64, 0, 6, 16, 16 );
		new FLEffect( x() + 20, y() + 6, 64, 0, 6, 16, 16 );
	}

	pound_frames = 0;
	jump_frames = 0;
	falling_frames = 0;
}

void FLPlayer::set_reset_position( float x, float y ) {
	reset_position.x = x;
	reset_position.y = y;
}

void FLPlayer::reset() {
	falling_frames = 0;

	FLWorldEnvironment::getInstance().mark_reset();
}

void FLPlayer::set_ability( FLPlayerAbility ability ) {
	cur_ability = ability;
}

void FLPlayer::interact() {
	FLWorldEnvironment::getInstance().interact( this );
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

