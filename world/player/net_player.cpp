/*
 * 	net_player.cpp
 *
 */

#include <iostream>
#include "net_player.h"
#include "../../net/fl_net.h"
#include "../../rendering/renderer.h"
#include "../../rendering/animated_object.h"

// TODO: this should probably be based on the terminal velocities in the player class
#define MAX_MOVEMENT 32.f
#define MS_PER_FRAME (1000.f/60.f)

FLNetPlayer::FLNetPlayer() : FLGameObject( 0, 0, 16, 32 ) {
	target.x = 0;
	target.y = 0;

	FLAnimatedObjectParams anim_params = { 5, 6, 4, 16.f, 32.f, true };
	FLTexturedObjectParams tex_params = { this, 0, 0, 16.f, 32.f };

	animators["body"] = new FLAnimatedObject( tex_params, anim_params );
	Renderer::getInstance().add_to_world( animators["body"] );

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
		set_x( tx );
		set_y( ty );
	}
	else {
		vel.x = (tx - x()) / elapsed_frames;
		vel.y = (ty - y()) / elapsed_frames;
	}
}

void FLNetPlayer::per_frame_update() {
	float dx = target.x - x();
	float dy = target.y - y();

	// if -1 < dx < 1
	if ( -1.f < dx && dx < 1.f) {
		vel.x = 0.f;
	}
	if ( -1.f < dy && dy < 1.f) {
		vel.y = 0.f;
	}

	move( vel.x, vel.y );
}

