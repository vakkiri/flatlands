/*
 * 	spor.cpp
 *
 */

#include <string>
#include "spor.h"
#include "common/fl_object.h"
#include "components/components.h"

namespace Spor {
	void clear() {
	}

	void init(fl_handle handle, float x, float y) {
		std::string surface_name = "world";

		FLObjects::set_pos(handle, x, y);
		FLObjects::add_texture(handle, "flower_texture", surface_name, "monsterplant_flower_idle", 12, -16);
		FLObjects::add_texture(handle, "stem_texture", surface_name, "monsterplant_base");
		FLObjects::add_animator(handle, "flower_animator", "flower_texture", "monsterplant_flower_idle", 6);
		FLObjects::add_collider(handle, "flower_collider", 12, -16, 32, 32);
		FLObjects::add_collision_group(handle, "flower_collider", "enemies");

	}

	void create(float x, float y) {
		fl_handle handle = FLObjects::create();
		
		if (handle != NULL_HANDLE) {
			init(handle, x, y);
		} else {
			std::cout << "Warning: could not create spor.\n";
		}
	}
}

