/*
 * 	player.cpp
 *
 */

#include "player.h"
#include "../../rendering/rendered_surface.h"
#include "../../logging/logging.h"

FLPlayer::FLPlayer(FLTexturedSurface* surface) : FLWorldObject(), FLTexturedObject() {
	this->surface = surface;
	position.x = 32;
	position.y = 64;
	position.w = 16;
	position.h = 16;
}

void FLPlayer::update_surface() {
	surface->update_buffers(this);
}

void FLPlayer::set_texture( texture *tex ) {
	surface->set_tex( tex );
}

