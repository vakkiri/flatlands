/*
 * 	fl_reep.cpp
 *
 */

#include <iostream>
#include <stdlib.h>

#include "../effect.h"
#include "../attack/fl_reep_attacks.h"
#include "../projectiles/fl_projectiles.h"
#include "../../components/components.h"
#include "../../rendering/animated_object.h"
#include "fl_reep.h"

#define W 32
#define H 32
#define S 0
#define T 288
#define ATTACK_INDICES 7
#define MAX_X_RAND 32

point attack_offsets[] = {{64, 0}, {0, 0}, {-64, -16}, {0, 0},
						  {0, 0},  {0, 0}, {0, 0}};
bool attacks_on[] = {true, true, true, false, false, false, false};

FLAnimatedObjectParams animation_params = {
	1,	  // num_animations
	8,	  // num_steps
	5,	  // frames_per_step
	32,	  // sstep
	32,	  // tstep
	true, // repeats
};

FLReep::FLReep(float x, float y) : FLMonster(x, y, W, H, animation_params) {
	animators["body"]->set_st(S, T);

	movement_period = 5;
	attack_period = 50;
	stun_duration = 120;
	vision_radius = 600;
	attack_index = rand() % 6;
	physics_handler()->set_gravity_factor(0.0f);
}

void FLReep::move() {
	float xv = physics_handler()->xvel();
	// float yv = physics_handler()->yvel();

	if (vector_from_player.x > 0) {
		animators["body"]->set_reverse(true);
		facing_right = false;
	} else if (vector_from_player.x < 0) {
		animators["body"]->set_reverse(false);
		facing_right = true;
	}
	if (vector_from_player.x > 64 && xv > -1.2f) {
		physics_handler()->accelerate(-0.64, 0);
	} else if (vector_from_player.x < -64 && xv < 1.2f) {
		physics_handler()->accelerate(0.64, 0);
	}

	if (vector_from_player.y > -94) {
		if (physics_handler()->yvel() > 0) {
			physics_handler()->accelerate(0.0, -0.3);
		} else {
			physics_handler()->accelerate(0.0, -0.1);
		}
	} else if (vector_from_player.y < -96) {
		if (physics_handler()->yvel() < 0) {
			physics_handler()->accelerate(0.0, 0.3);
		} else {
			physics_handler()->accelerate(0.0, 0.1);
		}
	}
}

void FLReep::per_frame_update() {
	FLMonster::per_frame_update();
	// if we aren't attacking now, but will attack soon, change animation
	if (attacks_on[attack_index]) {
		if ( attack_tick == 40 ) {
			animators["body"]->set_animation(1);
			animators["body"]->reset_animation();
		}
		if ( attack_tick < 40 ) {
			physics_handler()->stopx();
		}
	}
}

void FLReep::attack() {
	if (attacks_on[attack_index]) {
		animators["body"]->set_animation(0);

		if ( vector_from_player.x > 48 ) {
			attack1();
		}
		else {
			attack2();
		}
	}

	attack_index += 1;

	if (attack_index >= ATTACK_INDICES) {
		attack_index = 0;
	}
}

void FLReep::attack1() {
	float speed = 2.2f;

	point p = vector_from_player;
	p += attack_offsets[attack_index];

	float vx = -(p.x / distance_from_player) * speed;
	float vy = -(p.y / distance_from_player) * speed;

	new FLReepProjectile(x(), y() + 16, vx, vy);

}

void FLReep::attack2() {
	int xrand = rand() % MAX_X_RAND - (MAX_X_RAND / 2);
	new FLReepSecondary(x() - vector_from_player.x + xrand, y() - vector_from_player.y);
}

