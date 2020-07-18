/*
 * 	fl_monster.cpp
 *
 */

#include <math.h>

#include "../../components/components.h"
#include "../../environment/fl_environment.h"
#include "../../game/fl_game.h"
#include "../../rendering/renderer.h"
#include "../player/player.h"
#include "fl_monster.h"

FLMonster::FLMonster(float x, float y, float w, float h,
					 FLAnimatedObjectParams animation_params)
	: FLGameObject(x, y, w, h) {

	FLTexturedObjectParams tex_params = {shapes["position"], 0, 0, w, h};

	add_collider("position", "tilemap");
	fl_add_collider_to_group(colliders["tilemap"], "monsters");

	physics_handler_handle = new_physics_handler(this, "tilemap");
	updator_handle = new_updator(this);

	animators["body"] = new FLAnimatedObject(tex_params, animation_params);
	Renderer::getInstance().add_to_world(animators["body"]);
	monster_state = FL_MONSTER_IDLE;
	facing_right = true;

	movement_tick = 0;
	attack_tick = 0;
	stun_tick = 0;
	health = 0;
}

FLMonster::~FLMonster() {
	Renderer::getInstance().remove_from_world(animators["body"]);
}

point FLMonster::get_distance_from_player() {
	// TODO: choose closest of all net players
	FLPlayer *player = FLGame::instance().environment()->player();
	point p;
	p.x = x() - player->x();
	p.y = y() - player->y();
	return p;
}

void FLMonster::per_frame_update() {
	vector_from_player = get_distance_from_player();
	distance_from_player = sqrt((vector_from_player.x * vector_from_player.x) +
								(vector_from_player.y * vector_from_player.y));

	if (stun_tick > 0) {
		--stun_tick;
	}

	if (distance_from_player < vision_radius && stun_tick <= 0) {
		--movement_tick;
		--attack_tick;

		if (movement_tick <= 0) {
			move();
			movement_tick = movement_period;
		}

		if (attack_tick <= 0) {
			attack();
			attack_tick = attack_period;
		}

		if (distance_from_player < near_radius) {
			on_player_near();
		}
	}

	if (health <= 0) {
		delete this;
	}
}

void FLMonster::hit(float damage) {
	stun_tick = stun_duration;
	health -= damage;
}
