/*
 * 	xp_orb.cpp
 *
 */

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

static FLAnimatedObjectParams animation_params = {1, 7, 4, W, H, true};

FLXPOrb::FLXPOrb(float x, float y) : FLGameObject(x, y, W, H) {
	FLTexturedObjectParams tex_params = {shapes["position"], 0, 0, W, H};
	animators["body"] = new FLAnimatedObject(tex_params, animation_params);
	animators["body"]->set_st(S, T);
	Renderer::getInstance().add_to_world(animators["body"]);

	updator_handle = new_updator(this);
	add_collider("position", "tilemap");
	fl_add_collider_to_group(colliders["tilemap"], "projectiles");
	fl_get_collider(colliders["tilemap"])
		->set_collision_method(std::bind(&FLXPOrb::on_collision, this,
									 	std::placeholders::_1));

	ticks_since_created = 0;
	physics_handler_handle = new_physics_handler(this, "tilemap");
	physics_handler()->set_gravity_factor(0.01f);
	physics_handler()->set_friction_factor(0.1f);

	float vx = float((rand() % 80)) / 10.f - 2;
	float vy = float(-(rand() % 30)) / 10.f;
	physics_handler()->accelerate(vx, vy);
}

FLXPOrb::~FLXPOrb() {
	Renderer::getInstance().remove_from_world(animators["body"]);
}

void FLXPOrb::per_frame_update() {
	ticks_since_created += 1;
	if (ticks_since_created >= 50) {
		point d = distance_from_player();
		float mag = sqrt(d.x * d.x + d.y * d.y);
		float accel_amount = 0.1;
		if (mag <= 8) {
			delete this;
		} else if (mag != 0) {
			mag *= accel_amount;
			if (d.x < -4 || d.x > 4) {
				d.x /= mag;
			} else {
				d.x = 0;
				physics_handler()->stopx();
			}
			if (d.y < -4 || d.y > 4) {
				d.y /= mag;
			} else {
				d.y = 0;
				physics_handler()->stopy();
			}
			physics_handler()->accelerate(-d.x, -d.y);
		}
	}
}

void FLXPOrb::on_collision(FLCollider *obj) {
	(void) obj;
	if (obj != nullptr) {
		std::unordered_set<std::string>& collision_groups = obj->get_collision_groups();
		if (collision_groups.find("player") != collision_groups.end()) {
			std::cout << "playa\n";
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
