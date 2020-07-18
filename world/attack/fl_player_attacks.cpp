/*
 * 	fl_reep_attacks.cpp
 *
 */

#include <iostream>
#include "fl_player_attacks.h"
#include "../effect.h"
#include "../monster/fl_monster.h"
#include "../../components/components.h"

#define W 16
#define H 16

FLFusionPrimary::FLFusionPrimary(float x, float y, FLHorizontalDirection hdir, FLVerticalDirection vdir) : FLAttack(x, y, W, H, hdir, vdir) {
	pre_ticks = 0;
}

void FLFusionPrimary::attack() {
	fl_line collision_line;
	collision_line.u.x = x();
	collision_line.u.y = y();

	if (vdir == FL_UP) {
		collision_line.v.x = 0.f;
		collision_line.v.y = -1000.f;
	} else if (vdir == FL_DOWN) {
		collision_line.v.x = 0.f;
		collision_line.v.y = 1000.f;
	} else if (hdir == FL_LEFT) {
		collision_line.v.x = -1000.f;
		collision_line.v.y = 0.f;
	} else if (hdir == FL_RIGHT) {
		collision_line.v.x = 1000.f;
		collision_line.v.y = 0.f;
	}

	std::unordered_set<std::string> target_groups = std::unordered_set<std::string>({"tilemap", "monsters"});
	std::pair<point, FLGameObject*> hit_object = get_nearest_collision(&collision_line, target_groups);

	// hit_object.first.x  and hit_object.first.y will be -1 if there was no hit
	if (hit_object.first.x >= 0) {
		FLTexturedObjectParams tex_params = {nullptr, hit_object.first.x, hit_object.first.y, W, H};
		FLAnimatedObjectParams anim_params = {1, 6, 2, W, H, false};
		new FLEffect(tex_params, anim_params, 928, 80);

		// if an object was returned it was a monster, not tilemap
		if (hit_object.second != nullptr) {
			FLMonster* monster = static_cast<FLMonster*>(hit_object.second);
			monster->hit(10);
		}
	}
}

