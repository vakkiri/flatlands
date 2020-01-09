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

#define INITIAL_WALK_ACCEL (1.8)
#define WALK_ACCEL (0.54)
#define RUN_ACCEL (0.56)
#define JUMP_ACCEL (0.98)
#define X_TERMINAL_VELOCITY (4.2)
#define X_TERMINAL_WALK_VELOCITY (1.2)
#define Y_TERMINAL_VELOCITY (4.5)
#define JUMP_HOLD_GRAVITY_FACTOR (2.0)

FLPlayer::FLPlayer() : FLAnimatedObject( 4, 3, 10, 16 ) {
	this->surface = Renderer::getInstance().get_world_surface();

	position.x = 32;
	position.y = 64;
	position.w = 16;
	position.h = 16;

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
		accel.y = -JUMP_ACCEL;
		reset_animation();
		set_animation( 2 );
		on_ground_timer = 0;
	}
}

void FLPlayer::move_right() {
	// accelerate more if we do not have much momentum, to break past
	// the initial resistance of friction
	if ( vel.x > 0 && vel.x < 1 )
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
	if ( vel.x < 0 && vel.x > -1 )
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

	if ( vel.y > Y_TERMINAL_VELOCITY )
		vel.y = Y_TERMINAL_VELOCITY;
	else if ( vel.y < -Y_TERMINAL_VELOCITY )
		vel.y = -Y_TERMINAL_VELOCITY;
}

void FLPlayer::update_physics() {
	FLPhysicsObject::update_physics();
	bound_velocity();

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

void FLPlayer::release_jump() { jump_held = false; }

void FLPlayer::hold_run() { run_held = true; }

void FLPlayer::release_run() { run_held = false; }

