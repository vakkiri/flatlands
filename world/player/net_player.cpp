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
	last_real_pos.x = 0;
	last_real_pos.y = 0;

	target.x = 0;
	target.y = 0;

	// Add to renderer
	Renderer::getInstance().add_to_world(this);
	last_position_update = 0;
}

void FLNetPlayer::set_target( float tx, float ty ) {
	Uint32 tick = SDL_GetTicks();
	float dt;
	float elapsed_frames;
	float dx;
	float dy;
	float xerr;
	float yerr;

	// First, we calculate how many frames to repeat this. We do this by checking the interval
	// in ms between updates, and converting this to frames.
	// TODO: currently we just assume 60 fps
	dt = tick - last_position_update;
	last_position_update = tick;
	elapsed_frames = dt / MS_PER_FRAME;


	// Next, calculate the player's estimate velocity. We use the new position minus
	// the last real position. Since velocity is per frame we need to adjust this 
	// velocity based on how many frames have elapsed.
	dx = tx - last_real_pos.x;
	dy = ty - last_real_pos.y;
	last_vel.x = dx / elapsed_frames;
	last_vel.y = dy / elapsed_frames;

	// Now, set the target position. Assuming we will move about as much and in the same
	// direction as we did between the last updates, we just set the target to be the position
	// we received plus the last distance moved.
	// This target just sets the position we will stop at if we make it that far before the
	// next update.
	// We are more conservative with the target in the y direction since this is more prone to
	// sudden changes (jumping etc.)
	target.x = tx + dx * 2;
	target.y = ty + dy;

	// If the target is farther than the client could reasonably have moved. If this is the
	// case we just jump to the given position, and assume some kind of teleport happend (ie. a
	// level was started or a player respawned).
	if ( last_vel.x > MAX_MOVEMENT || last_vel.x < -MAX_MOVEMENT || last_vel.y > MAX_MOVEMENT || last_vel.y < - MAX_MOVEMENT ) {
		position.x = tx;
		position.y = ty;
	}
	// Otherwise, approximate how we expect the client to move before the next packet
	else {
		vel.x = (tx - x()) / elapsed_frames;
		vel.y = (ty - y()) / elapsed_frames;
	}

	last_real_pos.x = tx;
	last_real_pos.y = ty;
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

