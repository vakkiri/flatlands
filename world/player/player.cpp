/*
 * 	player.cpp
 *
 */

#include "player.h"

#include "../../input/input_handler.h"
#include "../../rendering/rendered_surface.h"
#include "../../logging/logging.h"

#define INITIAL_WALK_ACCEL (1.8)
#define WALK_ACCEL (0.5)
#define JUMP_ACCEL (4.5)
#define X_TERMINAL_VELOCITY (4.2)
#define Y_TERMINAL_VELOCITY (6.0)

FLPlayer::FLPlayer(FLTexturedSurface* surface) : FLAnimatedObject( 4, 3, 10, 16 ) {
	this->surface = surface;
	position.x = 32;
	position.y = 64;
	position.w = 16;
	position.h = 16;

	bind_actions();
	// TODO: Game specific animation settings should be loaded from
	// an external file.
	set_start_repeat(10, 0);
}

void FLPlayer::bind_actions() {
	// bind actions with player object
	std::function<void(void)> jump = std::bind(&FLPlayer::jump, this);
	std::function<void(void)> walk_left = std::bind(&FLPlayer::move_left, this);
	std::function<void(void)> walk_right = std::bind(&FLPlayer::move_right, this);

	// map binded actions to input handler
	FLInputHandler::getInstance().add_action(FL_KEY_ACTION2, FL_KEY_PRESSED, jump);
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
	if ( on_ground() )
		accelerate(point(0, -JUMP_ACCEL));
		reset_animation();
		set_animation( 2 );
}

void FLPlayer::move_right() {
	// accelerate more if we do not have much momentum, to break past
	// the initial resistance of friction
	if ( vel.x > 0 && vel.x < 1 )
		accelerate(point(INITIAL_WALK_ACCEL, 0));
	else
		accelerate(point(WALK_ACCEL, 0));

	// TODO: this should be based on an enumerated state
	set_animation(1);
	set_reverse(false);
}

void FLPlayer::move_left() {
	// accelerate more if we do not have much momentum, to break past
	// the initial resistance of friction
	if ( vel.x < 0 && vel.x > -1 )
		accelerate(point(-INITIAL_WALK_ACCEL, 0));
	else
		accelerate(point(-WALK_ACCEL, 0));

	// TODO: this should be based on an enumerated state
	set_animation(1);
	set_reverse(true);
}

void FLPlayer::bound_velocity() {
	if ( vel.x > X_TERMINAL_VELOCITY )
		vel.x = X_TERMINAL_VELOCITY;
	else if ( vel.x < -X_TERMINAL_VELOCITY )
		vel.x = -X_TERMINAL_VELOCITY;

	if ( vel.y > Y_TERMINAL_VELOCITY )
		vel.y = Y_TERMINAL_VELOCITY;
	else if ( vel.y < -Y_TERMINAL_VELOCITY )
		vel.y = -Y_TERMINAL_VELOCITY;
}

void FLPlayer::update_physics() {
	FLPhysicsObject::update_physics();
	bound_velocity();

	if ( !on_ground() )
		set_animation(2);
	else
		set_animation(0);
}

