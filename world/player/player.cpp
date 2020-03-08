/*
 * 	player.cpp
 *
 */

#include <iostream>

#include "player.h"

#include "../colliding_object.h"
#include "../effect.h"
#include "../physics_settings.h"
#include "../world_environment.h"
#include "../../input/input_handler.h"
#include "../../rendering/world_surface.h"
#include "../../rendering/renderer.h"
#include "../../logging/logging.h"

#define MAX_FALL (90)
#define INITIAL_WALK_ACCEL (0.7)
#define WALK_ACCEL (0.51)
#define RUN_ACCEL (0.54)

#define JUMP_ACCEL (0.29)
#define INITIAL_JUMP_VEL (-1.3)
#define JUMP_FRAME_ACCEL (0.03)
#define NUM_JUMP_FRAMES (10)

#define HOVER_FRAMES (40)
#define DOUBLE_JUMP_ACCEL (0.9)
#define GROUND_POUND_ACCEL (3.0)
#define POUND_FRAMES (60)

#define X_TERMINAL_VELOCITY (3.4)
#define X_TERMINAL_WALK_VELOCITY (3.2)
#define Y_TERMINAL_VELOCITY (6.9)
#define JUMP_HOLD_GRAVITY_FACTOR (2.5)

FLPlayer::FLPlayer() : FLAnimatedObject( 5, 3, 7, 16 ) {
	Renderer::getInstance().add_to_world(this);

	position.x = 32;
	position.y = 64;
	position.w = 16;
	position.h = 16;
	
	jump_frames = 0;
	hover_frames = 0;
	pound_frames = 0;
	falling_frames = 0;

	cur_ability = FL_GROUND_POUND;
	can_use_ability = false;

	jump_held = false;
	run_held = false;

	bind_actions();

	set_start_repeat(10, 0);
	// set_bounds_margin( rect{ 1, 1, -2, -2 } );
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

	// map binded actions to input handler
	FLInputHandler::getInstance().add_game_action(FL_KEY_ACTION1, FL_KEY_HELD, hold_run);
	FLInputHandler::getInstance().add_game_action(FL_KEY_ACTION1, FL_KEY_RELEASED, release_run);
	FLInputHandler::getInstance().add_game_action(FL_KEY_ACTION2, FL_KEY_PRESSED, jump);
	FLInputHandler::getInstance().add_game_action(FL_KEY_DOWN, FL_KEY_PRESSED, interact);
	FLInputHandler::getInstance().add_game_action(FL_KEY_ACTION2, FL_KEY_HELD, hold_jump);
	FLInputHandler::getInstance().add_game_action(FL_KEY_ACTION2, FL_KEY_RELEASED, release_jump);
	FLInputHandler::getInstance().add_game_action(FL_KEY_LEFT, FL_KEY_HELD, walk_left);
	FLInputHandler::getInstance().add_game_action(FL_KEY_RIGHT, FL_KEY_HELD, walk_right);
	FLInputHandler::getInstance().add_game_action(FL_KEY_RIGHT, FL_KEY_RELEASED, release_walk);
	FLInputHandler::getInstance().add_game_action(FL_KEY_LEFT, FL_KEY_RELEASED, release_walk);

}

void FLPlayer::jump() {
	if ( on_ground() ) {
		can_use_ability = true;
		hover_frames = 0;
		vel.y = INITIAL_JUMP_VEL;
		accel.y = -JUMP_ACCEL;
		reset_animation();
		on_ground_timer = 0;
		falling_frames = 0;
		jump_frames = NUM_JUMP_FRAMES;

		// Create a visual smoke effect
		new FLEffect( x() - (w()/2.f), y(), 64, 16, 5, 32, 16 );
	}
	else if ( can_use_ability ) {
		use_ability();
		can_use_ability = false;
	}
}


void FLPlayer::use_ability() {
	switch ( cur_ability ) {
		case FL_DOUBLE_JUMP:
			double_jump();
			break;
		case FL_GROUND_POUND:
			ground_pound();
			break;
		case FL_HOVER:
			hover();
		default:
			break;
	}
}

void FLPlayer::double_jump() {
	vel.y = 0;
	accel.y = -DOUBLE_JUMP_ACCEL;

	reset_animation();
}

void FLPlayer::ground_pound() {
	vel.y = 0;
	accel.y = GROUND_POUND_ACCEL;
	pound_frames = POUND_FRAMES;

	// create pound effect
	new FLEffect( x(), y(), 64, 0, 6, 16, 16 );
}

void FLPlayer::hover() {
	hover_frames = HOVER_FRAMES;
}

bool FLPlayer::pounding() {
	return pound_frames > 0;
}

void FLPlayer::move_right() {
	// accelerate more if we do not have much momentum, to break past
	// the initial resistance of friction
	if ( vel.x > 0 && vel.x < 1.5 && run_held )
		accelerate(point(INITIAL_WALK_ACCEL, 0));
	else if ( run_held )
		accelerate(point(RUN_ACCEL, 0));
	else
		accelerate(point(WALK_ACCEL, 0));

	set_reverse(false);
	state = FL_PLAYER_WALK;
}

void FLPlayer::move_left() {
	// accelerate more if we do not have much momentum, to break past
	// the initial resistance of friction
	if ( vel.x < 0 && vel.x > -1.5 && run_held )
		accelerate(point(-INITIAL_WALK_ACCEL, 0));
	else if ( run_held )
		accelerate(point(-RUN_ACCEL, 0));
	else
		accelerate(point(-WALK_ACCEL, 0));

	set_reverse(true);
	state = FL_PLAYER_WALK;
}

void FLPlayer::bound_velocity() { 
	if ( pound_frames > 0 )
		pound_frames--;
	else {
		if ( run_held ) {
			if ( vel.x > X_TERMINAL_VELOCITY )
				vel.x = X_TERMINAL_VELOCITY;
			else if ( vel.x < -X_TERMINAL_VELOCITY )
				vel.x = -X_TERMINAL_VELOCITY;
		}
		else {
			if ( vel.x > X_TERMINAL_WALK_VELOCITY )
				vel.x = X_TERMINAL_WALK_VELOCITY;
			else if ( vel.x < -X_TERMINAL_WALK_VELOCITY )
				vel.x = -X_TERMINAL_WALK_VELOCITY;
		}
	}

	if ( hover_frames > 0 ) {
		hover_frames--;
		vel.y = 0;
	}
	else {
		if ( vel.y > Y_TERMINAL_VELOCITY )
			vel.y = Y_TERMINAL_VELOCITY;
		else if ( vel.y < -Y_TERMINAL_VELOCITY )
			vel.y = -Y_TERMINAL_VELOCITY;
	}
}

void FLPlayer::update_physics() {
	FLPhysicsObject::update_physics();

	// reduce effect of gravity immediately after jumping
	if ( jump_frames > 0 ) {
		accel.y -= JUMP_FRAME_ACCEL;
		jump_frames--;
	}

	if ( on_ground() ) {
		hit_ground();
	}
	else if ( pounding() ) {
		state = FL_PLAYER_POUND;
		if ( pound_frames % 5 == 0 )
			new FLEffect( x(), y(), 64, 0, 6, 16, 16 );
	}
	else {
		state = FL_PLAYER_JUMP;

		if ( ++falling_frames > MAX_FALL )
			reset();
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

	float yoffset = -64;

	float dx = (r.world_camera_x() / 2) + x() + xoffset;
	float dy = (r.world_camera_y() / 2) + y() + yoffset;

	// TODO: replace hardcoded values with screen width/height * camera scale
	xamt = (dx / 50) * -X_TERMINAL_VELOCITY;
	yamt = (dy / 50) * -5.0;

	r.translate_world_camera( glm::vec3( xamt, yamt, 0 ) );
}

void FLPlayer::update_animation() {
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
		case FL_PLAYER_POUND:
			set_animation( 3 );
			break;
		default:
			break;
	}

	FLAnimatedObject::update_animation();
	update_camera();
}

void FLPlayer::apply_gravity() { 
	// The on_ground check is a slightly shoddy workaround to the fact that
	// there is an ugly vibrating effect when walking on ground with a low
	// value for gravity
	if ( !jump_held || on_ground() )
		accel.y += physics.gravity() * JUMP_HOLD_GRAVITY_FACTOR;
	else
		accel.y += physics.gravity();
}

void FLPlayer::hold_jump() { jump_held = true; }

void FLPlayer::release_jump() { 
	jump_held = false; 
	hover_frames = 0;
}

void FLPlayer::hold_run() { run_held = true; }

void FLPlayer::release_run() { run_held = false; }

void FLPlayer::release_walk() { state = FL_PLAYER_IDLE; }

void FLPlayer::hit_ground() {
	if ( state != FL_PLAYER_WALK )
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

