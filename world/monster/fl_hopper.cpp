/*
 * 	fl_hopper.cpp
 *
 */

#include "fl_hopper.h"
#include "../../components/components.h"
#include "../../rendering/animated_object.h"
#include "../misc/xp_orb.h"

#define W 32
#define H 32
#define S 304
#define T 80

static FLAnimatedObjectParams animation_params = {
	2,	// num_animations
	5,	// num_steps
	4,	// frames_per_step
	32,	// s step
	32,	// t step
	true	// repeats
};

FLHopper::FLHopper(float x, float y) : FLMonster(x, y, W, H, animation_params) {
	animators["body"]->set_st(S, T);
	animators["body"]->set_reverse(true);
	movement_period = 60;
	physics_handler()->set_gravity_factor(0.5f);
	health = 30;
	stun_duration = 30;
	xp = 3;
}

void FLHopper::per_frame_update() {
	FLMonster::per_frame_update();
}

