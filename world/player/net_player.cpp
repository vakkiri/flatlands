/*
 * 	net_player.cpp
 *
 */

#include <iostream>
#include "net_player.h"
#include "../../net/fl_net.h"
#include "../../rendering/renderer.h"

// TODO: this should probably be based on the terminal velocities in the player class
#define MAX_MOVEMENT 32.f
#define MS_PER_FRAME (1000.f/60.f)

FLNetPlayer::FLNetPlayer() : FLAnimatedObject( 5, 6, 4, 16.f, 32.f ) {
	position.x = 0;
	position.y = 0;
	position.w = 16;
	position.h = 32;

	target.x = 0;
	target.y = 0;

	// Add to renderer
	Renderer::getInstance().add_to_world(this);
	last_position_update = 0;
}

void FLNetPlayer::set_target( float tx, float ty ) {
	// Note that the player sends the EXPECTED position by the time this packet is
	// received, not the real position. Thus, it is just our job to smoothly move
	// to that position.
	Uint32 tick = SDL_GetTicks();
	Uint32 elapsed_ms = tick - last_position_update;
	last_position_update = tick;

	float elapsed_frames = (elapsed_ms) / MS_PER_FRAME;
	float dx = tx - x();
	float dy = ty - y();
	// per frame dx and dy
	float fdx = dx / elapsed_frames;
	float fdy = dy / elapsed_frames;

	target.x = tx;
	target.y = ty;

	// In the case of huge jumps (assumed to be from respawning or teleporting) we just
	// move instantly.
	if ( fdx > MAX_MOVEMENT || fdx < -MAX_MOVEMENT || fdy > MAX_MOVEMENT || fdy < -MAX_MOVEMENT ) {
		position.x = tx;
		position.y = ty;
	}
	else {
		vel.x = (tx - x()) / elapsed_frames;
		vel.y = (ty - y()) / elapsed_frames;
	}
}

void FLNetPlayer::update() {
	float dx = target.x - position.x;
	float dy = target.y - position.y;

	// if -1 < dx < 1
	if ( -1.f < dx && dx < 1.f) {
		vel.x = 0;
	}
	if ( -1.f < dy && dy < 1.f) {
		vel.y = 0;
	}

	if ( vel.x < 0 )
		set_reverse(true);
	else if ( vel.x > 0 )
		set_reverse(false);

	position.x += vel.x;
	position.y += vel.y;
}

