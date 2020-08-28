/*
 * 	fl_orb.cpp
 *
 */

#include <stdlib.h>
#include "fl_orb.h"

#include "../../components/components.h"
#include "../../rendering/animated_object.h"
#include "../objects/chip.h"

#define W 32
#define H 32
#define S 304
#define T 0

static FLAnimatedObjectParams animation_params = {
	2,	// num_animations
	8,	// num_steps
	4,	// frames_per_step
	32,	// sstep
	32, 	// tstep
	true	// repeats
};

FLOrb::~FLOrb() {
	if (!removed) {
		new FLChip(x() + w()/2.f, y() + h() /2.f);
	}
}

FLOrb::FLOrb(float x, float y) : FLMonster(x, y, W, H, animation_params) {
	animators["body"]->set_st(S, T);

	movement_period = 1;
	physics_handler()->set_gravity_factor(0.0f);
	health = 50;
	stun_duration = animation_params.num_steps * animation_params.frames_per_step;
	xp = 5;
}

void FLOrb::per_frame_update() {
	if (stun_tick <= 0)
		animators["body"]->set_animation(0);
	FLMonster::per_frame_update();
}

void FLOrb::hit(float damage) {
	FLMonster::hit(damage);
	animators["body"]->set_animation(1);
	animators["body"]->reset_animation();
}

