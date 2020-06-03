/*
 * 	fl_reep.cpp
 *
 */

#include <iostream>

#include "fl_reep.h"
#include "../../components/components.h"
#include "../projectiles/fl_projectiles.h"

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

	movement_period = 5;
	attack_period = 300;
	stun_duration = 120;
	vision_radius = 600;

	physics_handler()->accelerate( 2.f, 0 );
	physics_handler()->set_gravity_factor( 0.f );
}

void FLReep::move() {
	float xv = physics_handler()->xvel();
	float yv = physics_handler()->yvel();
	
	if ( vector_from_player.x > 0 ) {
		animators["body"]->set_reverse(true);
	}
	else if ( vector_from_player.x < 0 ) {
		animators["body"]->set_reverse(false);
	}
	if ( vector_from_player.x > 64 && xv > -1.2f ) {
		physics_handler()->accelerate(-0.76, 0);
	}
	else if ( vector_from_player.x < -64 && xv < 1.2f ) {
		physics_handler()->accelerate(0.76, 0);
	}
}

void FLReep::attack() {
	float speed = 1.5f;
	float vx = -(vector_from_player.x / distance_from_player) * speed;
	float vy = -(vector_from_player.y / distance_from_player) * speed;
	new FLReepProjectile( x(), y() + 16, vx, vy );
}

