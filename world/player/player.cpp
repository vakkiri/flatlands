/*
 * 	player.cpp
 *
 */

#include <iostream>

#include "player.h"

#include "../../input/input_handler.h"
#include "../../rendering/rendered_surface.h"
#include "../../logging/logging.h"

FLPlayer::FLPlayer(FLTexturedSurface* surface) : FLAnimatedObject( 4, 15, 16 ) {
	this->surface = surface;
	position.x = 32;
	position.y = 64;
	position.w = 16;
	position.h = 16;

	std::function<void(void)> jump = std::bind(&FLPlayer::jump, this);
	FLInputHandler::getInstance().add_action(FL_KEY_ACTION2, FL_KEY_PRESSED, jump);
}

void FLPlayer::update_surface() {
	surface->update_buffers(this);
}

void FLPlayer::set_texture( texture *tex ) {
	surface->set_tex( tex );
}

void FLPlayer::jump() {
	std::cout << " JUMP ! \n";
}

