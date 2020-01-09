/*
 * 	player.cpp
 *
 */

#include <iostream>

#include "player.h"

#include "../physics_settings.h"
#include "../../input/input_handler.h"
#include "../../rendering/rendered_surface.h"
#include "../../rendering/renderer.h"
#include "../../logging/logging.h"

#define INITIAL_WALK_ACCEL (2.0)
#define WALK_ACCEL (0.54)
#define RUN_ACCEL (0.59)

#define JUMP_ACCEL (0.7)
#define JUMP_FRAME_ACCEL (0.04)
#define NUM_JUMP_FRAMES (3)

#define HOVER_FRAMES (40)
#define DOUBLE_JUMP_ACCEL (0.8)
#define GROUND_POUND_ACCEL (3.0)
#define POUND_FRAMES (90)

#define X_TERMINAL_VELOCITY (4.2)
#define X_TERMINAL_WALK_VELOCITY (1.6)
#define Y_TERMINAL_VELOCITY (7.0)
#define JUMP_HOLD_GRAVITY_FACTOR (2.0)

FLPlayer::FLPlayer() : FLAnimatedObject( 4, 3, 10, 16 ) {
	this->surface = Renderer::getInstance().get_world_surface();

	position.x = 32;
	position.y = 64;
	position.w = 16;
	position.h = 16;

	jump_frames = 0;
	cur_ability = FL_GROUND_POUND;
	can_use_ability = false;

	bind_actions();

	// TODO: Game specific animation settings should be loaded from
	// an external file.
	set_texture( FLResources::getInstance().get_image("neko_idle") );
	update_surface();
	set_start_repeat(10, 0);
}

void FLPlayer::bind_actions() {
	// bind actions with player object
	std::function<void(void)> jump = std::bind(&FLPlayer::jump, this);
	std::function<void(void)> hold_jump = std::bind(&FLPlayer::hold_jump, this);
	std::function<void(void)> release_jump = std::bind(&FLPlayer::release_jump, this);
	std::function<void(void)> hold_run = std::bind(&FLPlayer::hold_run, this);
	std::function<void(void)> release_run = std::bind(&FLPlayer::release_run, this);
	std::function<void(void)> walk_left = std::bind(&FLPlayer::move_left, this);
	std::function<void(void)> walk_right = std::bind(&FLPlayer::move_right, this);

	// map binded actions to input handler
	FLInputHandler::getInstance().add_action(FL_KEY_ACTION1, FL_KEY_HELD, hold_run);
	FLInputHandler::getInstance().add_action(FL_KEY_ACTION1, FL_KEY_RELEASED, release_run);
	FLInputHandler::getInstance().add_action(FL_KEY_ACTION2, FL_KEY_PRESSED, jump);
	FLInputHandler::getInstance().add_action(FL_KEY_ACTION2, FL_KEY_HELD, hold_jump);
	FLInputHandler::getInstance().add_action(FL_KEY_ACTION2, FL_KEY_RELEASED, release_jump);
	FLInputHandler::getInstance().add_action(FL_KEY_LEFT, FL_KEY_HELD, walk_left);
	FLInputHandler::getInstance().add_action(FL_KEY_RIGHT, FL_KEY_HELD, walk_right);

}

void FLPlayer::update_surface() {
	surface->update_buffers(this);
}

void FLPlayer::set_texture( texture *tex ) {
	surface->set_tex( tex );
}

void FLPlayer::jump() {
	if ( on_ground() ) {
		can_use_ability = true;
		hover_frames = 0;
		accel.y = -JUMP_ACCEL;
		reset_animation();
		set_animation( 2 );
		on_ground_timer = 0;
		jump_frames = NUM_JUMP_FRAMES;
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
	set_animation( 2 );
}

void FLPlayer::ground_pound() {
	vel.x = 0;
	accel.x = 0;
	vel.y = 0;
	accel.y = GROUND_POUND_ACCEL;
	pound_frames = POUND_FRAMES;
}

void FLPlayer::hover() {
	hover_frames = HOVER_FRAMES;
}

void FLPlayer::move_right() {
	// accelerate more if we do not have much momentum, to break past
	// the initial resistance of friction
	if ( vel.x > 0 && vel.x < 1.5 )
		accelerate(point(INITIAL_WALK_ACCEL, 0));
	else if ( run_held )
		accelerate(point(RUN_ACCEL, 0));
	else
		accelerate(point(WALK_ACCEL, 0));

	// TODO: this should be based on an enumerated state
	if ( on_ground() )
		set_animation(1);

	set_reverse(false);
}

void FLPlayer::move_left() {
	// accelerate more if we do not have much momentum, to break past
	// the initial resistance of friction
	if ( vel.x < 0 && vel.x > -1.5 )
		accelerate(point(-INITIAL_WALK_ACCEL, 0));
	else if ( run_held )
		accelerate(point(-RUN_ACCEL, 0));
	else
		accelerate(point(-WALK_ACCEL, 0));

	// TODO: this should be based on an enumerated state
	if ( on_ground() )
		set_animation(1);

	set_reverse(true);
}

void FLPlayer::bound_velocity() { 
	if ( pound_frames > 0 ) {
		pound_frames--;
		vel.x = 0;
		accel.x = 0;
	}
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
		pound_frames = 0;
	}

	bound_velocity();

	update_position();
	// TODO: animation update should not be part of physics update
	if ( !on_ground() )
		set_animation(2);
	else
		set_animation(0);

	// TODO: camera update should not be part of physics update
	update_camera();
}

void FLPlayer::update_camera() {
	Renderer& r = Renderer::getInstance();
	float xamt = 0;
	float yamt = 0;
	float dx = (r.world_camera_x() / 2) + x();
	float dy = (r.world_camera_y() / 2) + y();

	// TODO: replace hardcoded values with screen width/height * camera scale
	xamt = (dx / 50) * -5.0;
	yamt = (dy / 50) * -5.0;

	r.translate_world_camera( glm::vec3( xamt, yamt, 0 ) );
}

void FLPlayer::apply_gravity() { 
	if ( !jump_held )
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

