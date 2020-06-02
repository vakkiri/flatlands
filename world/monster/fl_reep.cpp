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
	8,	// num_steps
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
	physics_handler()->set_gravity_factor( 0.f );
}

void FLReep::per_frame_update() {
	float xv = physics_handler()->xvel();
	float yv = physics_handler()->yvel();
	
	point p = distance_from_player();

	if ( p.x > 0 ) {
		animators["body"]->set_reverse(true);
	}
	else if ( p.x < 0 ) {
		animators["body"]->set_reverse(false);
	}
	if ( p.x > 64 && xv > -1.2f ) {
		physics_handler()->accelerate(-0.154, 0);
	}
	else if ( p.x < -64 && xv < 1.2f ) {
		physics_handler()->accelerate(0.154, 0);
	}
}

void FLReep::attack() {
}
