/*
 * 	fl_hopper.cpp
 *
 */

#include <iostream>
#include "fl_hopper.h"
#include "../../components/components.h"
#include "../../rendering/animated_object.h"
#include "../misc/xp_orb.h"

#define W 22
#define H 22
#define S 304
#define T 80

#define JUMP_X		-2.5
#define JUMP_Y		-3.25
#define HOP_PERIOD	65
#define WARNING_X	16

static FLAnimatedObjectParams animation_params = {
	2,	// num_animations
	7,	// num_steps
	6,	// frames_per_step
	22,	// s step
	22,	// t step
	true	// repeats
};

FLHopper::FLHopper(float x, float y) : FLMonster(x, y, W, H, animation_params) {
	animators["body"]->set_st(S, T);
	animators["body"]->set_reverse(true);
	physics_handler()->set_gravity_factor(0.4f);
	physics_handler()->set_friction_factor(0.3f);
	health = 30;
	stun_duration = 30;
	xp = 3;
	near_radius = 70;
	vision_radius = 140;
	attack_period = 240;
	movement_period = 1;
}

void FLHopper::per_frame_update() {
	// the hop period - 10 thing is an ugly hack to avoid jumping multiple frames in a row
	// sorry not sorry
	if (physics_handler()->on_ground() && 
			movement_tick <= HOP_PERIOD - 5 &&
			stun_tick <= 1) {
		if (vector_from_player.x >= -vision_radius -WARNING_X &&
				vector_from_player.x <= vision_radius + WARNING_X) {

			animators["body"]->set_animation(2);
			animators["body"]->set_repeats(true);
		} else {
			animators["body"]->set_animation(0);
			animators["body"]->set_repeats(true);
		}
	}
	FLMonster::per_frame_update();
}

void FLHopper::on_player_near() {
	if (physics_handler()->on_ground() && movement_tick <= 1) {
		movement_tick = HOP_PERIOD;
		animators["body"]->set_animation(1);
		animators["body"]->reset_animation();
		physics_handler()->accelerate(0, JUMP_Y);
	}
}

void FLHopper::attack() {
	if (physics_handler()->on_ground()) {
		movement_tick = HOP_PERIOD;
		animators["body"]->set_animation(1);
		animators["body"]->reset_animation();
		if (vector_from_player.x < 0) {
			physics_handler()->accelerate(-JUMP_X, JUMP_Y);
		} else {
			physics_handler()->accelerate(JUMP_X, JUMP_Y);
		}
	} else {
		// don't waste the attack if we couldn't use it
		attack_tick = 1;
	}
}

void FLHopper::hit(float damage) {
	FLMonster::hit(damage);
	animators["body"]->set_animation(3);
	animators["body"]->reset_animation();
}
