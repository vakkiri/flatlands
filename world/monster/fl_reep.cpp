/*
 * 	fl_reep.cpp
 *
 */

#include <iostream>

#include "fl_reep.h"
#include "../../components/components.h"

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


FLReep::FLReep( float x, float y ) : FLMonster( x, y, W, H, animation_params ) {
	animators["body"]->set_st( S, T );
	movement_period = 1;
	stun_duration = 120;
	physics_handler()->accelerate( 2.f, 0 );
}

void FLReep::update() {
}

void FLReep::move() {
}

void FLReep::attack() {
}
