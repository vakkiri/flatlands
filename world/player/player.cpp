/*
 * 	player.cpp
 *
 */

#include "player.h"

#include "../../input/input_handler.h"
#include "../../rendering/rendered_surface.h"
#include "../../logging/logging.h"

#define WALK_ACCEL (0.5)

FLPlayer::FLPlayer(FLTexturedSurface* surface) : FLAnimatedObject( 4, 15, 16 ) {
	this->surface = surface;
	position.x = 32;
	position.y = 64;
	position.w = 16;
	position.h = 16;

	bind_actions();
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
		accelerate(point(0, -4.0));
}

void FLPlayer::move_right() {
	accelerate(point(WALK_ACCEL, 0));
}

void FLPlayer::move_left() {
	accelerate(point(-WALK_ACCEL, 0));
}


