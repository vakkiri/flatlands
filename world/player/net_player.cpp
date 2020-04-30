/*
 * 	net_player.cpp
 *
 */

#include "net_player.h"
#include "../../net/fl_net.h"
#include "../../rendering/renderer.h"

FLNetPlayer::FLNetPlayer() : FLAnimatedObject( 5, 6, 7, 16.f, 32.f ) {
	position.x = 0;
	position.y = 0;
	position.w = 16;
	position.h = 32;

	target.x = 0;
	target.y = 0;

	// Add to renderer
	Renderer::getInstance().add_to_world(this);
}

void FLNetPlayer::set_target( float tx, float ty ) {
	target.x = tx;
	target.y = ty;

	float dx = target.x - position.x;
	float dy = target.y - position.y;

	vel.x = dx / 16.f;
	vel.y = dy / 16.f;
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

