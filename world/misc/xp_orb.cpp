/*
 * 	xp_orb.cpp
 *
 */

#include <algorithm>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "xp_orb.h"
#include "../../components/components.h"
#include "../../environment/fl_environment.h"
#include "../../game/fl_game.h"
#include "../../rendering/renderer.h"
#include "../../rendering/animated_object.h"
#include "../player/player.h"

#define S 304
#define T 64
#define W 8
#define H 8

static std::vector<FLXPOrb*> orbs;

static FLAnimatedObjectParams animation_params = {1, 7, 4, W, H, true};

FLXPOrb::FLXPOrb(float x, float y) : FLGameObject(x, y, W, H) {
	FLTexturedObjectParams tex_params = {shapes["position"], 0, 0, W, H};
	animators["body"] = new FLAnimatedObject(tex_params, animation_params);
	animators["body"]->set_st(S, T);
	Renderer::getInstance().add_to_world(animators["body"]);

	updator_handle = new_updator(this);
	add_collider("position", "tilemap");
	fl_add_collider_to_group(colliders["tilemap"], "projectiles");

	ticks_since_created = 0;
	physics_handler_handle = new_physics_handler(this, "tilemap");
	physics_handler()->set_gravity_factor(0.01f);
	physics_handler()->set_friction_factor(0.1f);

	float vx = float((rand() % 80)) / 10.f - 2;
	float vy = float(-(rand() % 30)) / 10.f;
	physics_handler()->accelerate(vx, vy);

	orbs.push_back(this);
}

FLXPOrb::~FLXPOrb() {
	orbs.erase(std::remove(orbs.begin(), orbs.end(), this), orbs.end());
	Renderer::getInstance().remove_from_world(animators["body"]);
}

void FLXPOrb::per_frame_update() {
	ticks_since_created += 1;
	if (ticks_since_created >= 50) {
		point d = distance_from_player();
		float mag = sqrt(d.x * d.x + d.y * d.y);
		float v = 5;
		if (mag <= 8) {
			FLPlayer *player = FLGame::instance().environment()->player();
			player->add_fragment();
			delete this;
		} else if (mag != 0) {
			physics_handler()->stopx();
			physics_handler()->stopy();
			d.x /= mag;
			d.y /= mag;
			d.x *= v;
			d.y *= v;
			physics_handler()->accelerate(-d.x, -d.y);
		}
	}
}

point FLXPOrb::distance_from_player() {
	FLPlayer *player = FLGame::instance().environment()->player();
	point p;
	p.x = x() - player->x();
	p.y = y() - player->y();
	return p;
}

void clear_xp_orbs() {
	while (!orbs.empty()) {
		delete orbs.back();	// this will remove from the vector as well!!!
	}
}

