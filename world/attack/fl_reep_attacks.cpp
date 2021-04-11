/*
 * 	fl_reep_attacks.cpp
 *
 */

#include <iostream>
#include "fl_reep_attacks.h"
#include "../effect.h"
#include "../player/player.h"
#include "../../environment/fl_environment.h"
#include "../../game/fl_game.h"
#include "../../utils/collision_utils.h"

#define W 32
#define H 32

FLReepSecondary::FLReepSecondary(float x, float y) : FLAttack(x, y, W, H, FL_NONE, FL_FORWARD) {
	pre_ticks = 56;
}

void FLReepSecondary::pre_attack() {
	if (tick == 0) {
		// Create effect
	}
}

void FLReepSecondary::attack() {
	FLPlayer* p = FLGame::instance().environment()->player();
	point center;
	center.x = x() + (w() / 2.f);
	center.y = y() + (h() / 2.f);
	
	if (point_in_shape(p->get_shape("position"), &center)) {
		p->hit(75);
	}
}

