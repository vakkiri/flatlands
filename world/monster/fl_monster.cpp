/*
 * 	fl_monster.cpp
 *
 */

#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include "../../components/components.h"
#include "../../environment/fl_environment.h"
#include "../../game/fl_game.h"
#include "../../rendering/renderer.h"
#include "../effect.h"
#include "../misc/xp_orb.h"
#include "../player/player.h"
#include "fl_monster.h"

static std::vector<FLMonster*> monsters;

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
	vision_radius = 0;
	attack_tick = 0;
	stun_tick = 0;
	health = 0;
	xp = 1;

	removed = false;
	monsters.push_back(this);
}

FLMonster::~FLMonster() {
	// this is super not efficient at all but i don't expect to have many monsters...
	monsters.erase(std::remove(monsters.begin(), monsters.end(), this), monsters.end());

	// monsters can be removed from play, such that they don't spawn xp etc. when deleted
	if (!removed) {
		// generate some generic effects
		int num_explosions =  w() / 8.f;
		if (num_explosions < 1) {
			num_explosions = 1;
		}
		srand(FLGame::instance().environment()->player()->x()); // idk this is kind of an odd choice
		for (int i = 0; i < num_explosions; ++i) {
			float _x = x() + (w() / 2.f);
			float _y = y() + (h() / 2.f);
			unsigned int speed = 3 + rand() % 4;
			_x += (rand() % 32) - 16;
			_y += (rand() % 32) - 16;
			// TODO I should really just have a struct that contains these settings defined
			// ie. effects.h can include them
			FLTexturedObjectParams tex_params = {nullptr, _x, _y, 16, 16};
			FLAnimatedObjectParams anim_params = {1, 6, speed, 16, 16, false};
			new FLEffect(tex_params, anim_params, 560, 0);
		}
		for (int i = 0; i < xp; ++i) {
			int dx = (rand() % 16) - 8;
			int dy = (rand() % 16) - 8;
			new FLXPOrb(x() + w() / 2.f + dx, y() + h() / 2.f + dy);
		}
	}

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

	if (movement_tick > 0) {
		--movement_tick;
	}

	if (distance_from_player < vision_radius && stun_tick <= 0) {
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

point FLMonster::get_vector_from_player() {
	return vector_from_player;
}

void FLMonster::remove_from_play() {
	removed = true;
}

bool FLMonster::removed_from_play() {
	return removed;
}

void clear_monsters() {
	while (!monsters.empty()) {
		monsters.back()->remove_from_play();
		delete monsters.back();
	}
}

