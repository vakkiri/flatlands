/*
 * 	fl_reep.cpp
 *
 */

#include <iostream>

#include "fl_reep.h"

#define W 	32
#define H 	32
#define S 	0
#define T 	288


FLAnimatedObjectParams animation_params = {
	1,	// num_animations
	10,	// num_steps
	5,	// frames_per_step
	32,	// sstep
	32,	// tstep
	true,	// repeats
};

FLReep::FLReep( float x, float y ) : 
	FLGameObject( x, y, W, H ),
	FLMonster( x, y, W, H, animation_params ) {
	set_st( S, T );
	movement_period = 1;
	stun_duration = 120;
	vel.x = 1.f;
}

void FLReep::update() {
}

void FLReep::move() {
}

void FLReep::attack() {
}
