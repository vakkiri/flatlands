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

FLFusionPrimary::FLFusionPrimary(float x, float y) : FLAttack(x, y, W, H) {
	pre_ticks = 0;
}

void FLFusionPrimary::attack() {
	fl_line collision_line;
	collision_line.u.x = x();
	collision_line.u.y = y();
	collision_line.v.x = 0.f;
	collision_line.v.y = -1000.f;

	std::unordered_set<std::string> target_groups = std::unordered_set<std::string>({"tilemap", "monsters"});
	std::pair<point, FLGameObject*> hit_object = get_nearest_collision(&collision_line, target_groups);

	if (hit_object.first.x >= 0) {
		FLTexturedObjectParams tex_params = {nullptr, hit_object.first.x, hit_object.first.y, W, H};
		FLAnimatedObjectParams anim_params = {1, 7, 2, W, H, false};
		new FLEffect(tex_params, anim_params, 0, 256);

		// if an object was returned it was a monster, not tilemap
		if (hit_object.second != nullptr) {
			FLMonster* monster = static_cast<FLMonster*>(hit_object.second);
			monster->hit(10);
		}
	}

}

