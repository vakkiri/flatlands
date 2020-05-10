/*
 * 	platform.cpp
 *
 */

#include <iostream>

#include "platform.h"

#include "../../rendering/renderer.h"
#include "../player/player.h"
#include "../world_environment.h"

#define NUM_ANIMATIONS 1
#define NUM_STEPS 2
#define FRAMES_PER_STEP 5
#define STEP 64
#define REPEATS true

#define S 0
#define T 192
#define W 64
#define H 16

#define DX 128

#define FRICTION_FACTOR (1.0)

FLPlatform::FLPlatform( float x, float y ) :
	FLGameObject( x, y, W, H ),
	FLAnimatedObject(
			NUM_ANIMATIONS,
			NUM_STEPS,
			FRAMES_PER_STEP,
			STEP,
			REPEATS
			) {
	set_st( S, T );

	Renderer::getInstance().add_to_world( this );
	left.x = x - DX;
	right.x = x + DX;
	left.y = y;
	right.y = y;

	vel.x = 1.f;
	vel.y = 0.f;
}

FLPlatform::~FLPlatform() {
	Renderer::getInstance().remove_from_world( this );
}

void FLPlatform::collide_with() {
	FLPlayer* player = FLWorldEnvironment::getInstance().player();
	if ( vel.y <= 0 )
		vel.y += player->get_vel().y * 0.9;

	player->set_y( y() - player->h() );
	player->set_on_ground();

	player->movex( vel.x * FRICTION_FACTOR );
}

void FLPlatform::update() {
	move( vel.x, vel.y );

	if ( y() > left.y)
		vel.y -= 0.4;
	else if ( y() < left.y ) {
		vel.y = 0;
		set_y( left.y );
	}

	if ( x() > right.x || x() < left.x )
		vel.x *= -1.f;
}

