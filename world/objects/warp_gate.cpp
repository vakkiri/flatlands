/*
 * 	warp_gate.cpp
 *
 */

#include <iostream>

#include "warp_gate.h"

#include "../world_environment.h"
#include "../../rendering/renderer.h"

#define NUM_ANIMATIONS 1
#define NUM_STEPS 3
#define FRAMES_PER_STEP 10
#define STEP 32
#define REPEATS true

#define S 0
#define T 112
#define W 32
#define H 48

FLWarpGate::FLWarpGate( float x, float y ) :
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
	FLWorldEnvironment::getInstance().add_colliding_object( this );

}

FLWarpGate::~FLWarpGate() {
	Renderer::getInstance().remove_animated_object( this );
	Renderer::getInstance().remove_from_world( this );
}

void FLWarpGate::collide_with( FLPlayer *player ) {
	FLWorldEnvironment::getInstance().load_next_level();
}


