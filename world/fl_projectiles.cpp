/*
 * 	fl_projectiles.cpp
 *
 */

#include <iostream>
#include <unordered_map>
#include "common/fl_object.h"
#include "components/components.h"
#include "fl_projectiles.h"

namespace FLProjectiles {
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
		life -= 1;

		// TODO: this should be done by the physics body
		float vx = std::get<float>(projectile.vars["vx"]);
		float vy = std::get<float>(projectile.vars["vy"]);
		
		projectile.x += vx;
		projectile.y += vy;

		if (life <= 0) {
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
		// TODO: this should be replaced by a physics body
		FLObjects::add_var(handle, "vx", vx);
		FLObjects::add_var(handle, "vy", vy);

		FLObjects::add_script(handle, update);

		// TODO: setup collider info
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

