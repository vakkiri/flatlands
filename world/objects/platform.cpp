/*
 * 	platform.cpp
 *
 */

#include <iostream>

#include "platform.h"

#include "../../rendering/renderer.h"
#include "../player/player.h"

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

	moving_right = false;
}

FLPlatform::~FLPlatform() {
	Renderer::getInstance().remove_from_world( this );
}

void FLPlatform::collide_with( FLPlayer *player ) {
}

void FLPlatform::update() {
	// TODO: smoother physics for movement; ease in/out and pause at ends
	if ( moving_right )
		movex( 0.75 );
	else
		movex( -0.75 );

	if ( x() > right.x || x() < left.x )
		moving_right = !moving_right;
}

