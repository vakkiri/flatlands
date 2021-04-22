/*
 * 	fl_projectiles.cpp
 *
 */

#include <iostream>
#include <stdlib.h>
#include <unordered_map>
#include "common/fl_object.h"
#include "components/components.h"
#include "fl_projectiles.h"
#include "world/effect.h"

// TODO: move this somewhere else
// TODO: on death should even probably be moved to FLObject
void fusion_impact(FLObject& projectile) {
	float x = FLObjects::x(projectile.handle);
	float y = FLObjects::y(projectile.handle);
	float w = 8;
	float h = 4;
	for (int i = 0; i < 5; ++i) {
		float _x = x + w;
		float _y = y + h;
		unsigned int speed = 3 + rand() % 4;
		_x += (rand() % 16) - 8;
		_y += (rand() % 16) - 8;
		FLEffects::create(_x, _y, "fusion_impact", speed);
	}
}

namespace FLProjectiles {
	// Pre-defined projectile "templates" from assets/projectiles.conf
	std::unordered_map<std::string, FLProjectileInfo> projectiles;

	void define(
		std::string name,
		float v,
		float x,
		float y,
		float w,
		float h,
		int damage,
		int life,
		std::string collection
	){
		projectiles[name] = FLProjectileInfo {
			v, x, y, w, h, damage, life, collection
		};
	}

	void update(FLObject& projectile) {
		int life = std::get<int>(projectile.vars["life"]);
		fl_handle body = projectile.physics_body;
		life -= 1;

		if (life <= 0 || FLPhysicsBodies::touched_tilemap(body)) {
			FLObjects::destroy(projectile.handle);
			return;
		}

		projectile.vars["life"] = life;
	}

	void init(
		fl_handle handle,
		float vx,
		float vy,
		float x,
		float y,
		int bonus_damage,
		std::string type,
		bool player_is_source
	) {
		std::string tex_name = "texture";
		std::string ani_name = "animation";
		std::string col_name = "collider";
		std::string surface_name = "world";

		int frame_length = 4;

		FLProjectileInfo& info = projectiles[type];

		FLObjects::set_pos(handle, x, y);
		FLObjects::add_texture(handle, tex_name, surface_name, info.collection);
		FLObjects::add_animator(handle, ani_name, tex_name, info.collection, frame_length);
		FLObjects::add_collider(handle, col_name, info.x, info.y, info.w, info.h);
		FLObjects::add_var(handle, "damage", info.damage + bonus_damage);
		FLObjects::add_var(handle, "life", info.life);
		FLObjects::add_physics_body(handle, "collider");
		FLObjects::accelerate(handle, vx, vy);

		FLObjects::add_script(handle, update);

		// TODO: death script name should be taken from config
		FLObjects::add_death_script(handle, fusion_impact);

		if (!player_is_source) {
			FLObjects::add_collision_target(handle, col_name, "player");
		} else {
			FLObjects::add_collision_target(handle, col_name, "enemies");
		}

	}

	void create(
		float x,
		float y,
		std::string type,
		bool player_is_source,
		FLDirection direction
	) {
		fl_handle handle = FLObjects::create();

		if (handle != NULL_HANDLE) {
			float vx, vy;
			FLProjectileInfo& info = projectiles[type];
			vx = vy = 0;

			if (direction == DIR_UP) {
				vy = -info.v;
			} else if (direction == DIR_DOWN) {
				vy = info.v;
			} else if (direction == DIR_LEFT) {
				vx = -info.v;
			} else if (direction == DIR_RIGHT) {
				vx = info.v;
			}

			init(handle, vx, vy, x, y, 0, type, player_is_source);
		}
	}
}

