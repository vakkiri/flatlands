/*
 * 	spor.cpp
 *
 */

#include <iostream>
#include <vector>
#include <string>
#include "spor.h"
#include "common/fl_object.h"
#include "components/components.h"

namespace Spor {
	std::vector<fl_handle> spors;

	void clear() {
		while (!spors.empty()) {
			FLObjects::destroy(spors.back());
			spors.pop_back();
		}
	}

	void update(FLObject& spor) {
		int alive = std::get<int>(spor.vars["alive"]);
		int health = std::get<int>(spor.vars["health"]);
		if (alive) {
			if (health <= 0) {
				alive = false;
				FLTextures::set_visible(spor.textures["flower_texture"], false);
				FLObjects::remove_collider(spor.handle, "flower_collider");
			}
		}
		spor.vars["alive"] = alive;
	}

	void init(fl_handle handle, float x, float y) {
		std::string surface_name = "world";

		FLObjects::set_pos(handle, x, y);
		FLObjects::add_texture(handle, "flower_texture", surface_name, "monsterplant_flower_idle", 12, -16);
		FLObjects::add_texture(handle, "stem_texture", surface_name, "monsterplant_base");
		FLObjects::add_animator(handle, "flower_animator", "flower_texture", "monsterplant_flower_idle", 6);
		FLObjects::add_collider(handle, "flower_collider", 12, -16, 32, 24);
		FLObjects::add_collision_group(handle, "flower_collider", "enemies");
		FLObjects::add_var(handle, "health", 500);
		FLObjects::add_var(handle, "alive", 1);
		FLObjects::add_script(handle, update);
	}

	void create(float x, float y) {
		fl_handle handle = FLObjects::create();
		
		if (handle != NULL_HANDLE) {
			init(handle, x, y);
			spors.push_back(handle);
		} else {
			std::cout << "Warning: could not create spor.\n";
		}
	}
}

