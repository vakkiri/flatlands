/*
 * 	fl_colliders.cpp
 *
 */

#include <algorithm>
#include <iostream>
#include "fl_colliders.h"
#include "common/fl_static_buffer.h"
#include "utils/collision_utils.h"
#include "environment/fl_environment.h"
#include "game/fl_game.h"
#include "tilemap/tilemap.h"

#define INITIAL_COLLIDERS 5000

namespace FLColliders {
	FLStaticBuffer<FLCollisionBox> colliders(INITIAL_COLLIDERS);
	std::unordered_map<std::string, std::unordered_set<fl_handle>> groups;

	bool collides(FLCollisionBox& a, FLCollisionBox& b) {
		float ax = a.x + FLObjects::x(a.handle);
		float ay = a.y + FLObjects::y(a.handle);
		float aw = a.w;
		float ah = a.h;
		float bx = b.x + FLObjects::x(b.handle);
		float by = b.y + FLObjects::y(b.handle);
		float bw = b.w;
		float bh = b.h;

		return rect_collision(ax, ay, aw, ah, bx, by, bw, bh);
	}

	bool touches_tilemap(fl_handle handle) {
		float endx = colliders[handle].x + colliders[handle].w;
		float endy = colliders[handle].y + colliders[handle].h;
		FLTilemap* tilemap = FLGame::instance().environment()->tilemap();
	
		// TODO: 16 should be based on tile size and not hardcoded	
		for (float x = colliders[handle].x; x <= endx; x += 16) {
			for (float y = colliders[handle].y; y <= endy; y += 16) {
				float _x = std::min(x, endx);
				float _y = std::min(y, endy);

				// This won't rly work with the per pixel
				// solid maps, but should hit all normal tiles
				if (tilemap->solid_at(_x, _y)) {
					return true;
				}
			}
		}

		return false;
	}

	void find_collisions() {
		for (FLCollisionBox& a : colliders) {
			for (auto group_name : a.target_groups) {
				for (auto handle : groups[group_name]) {
					FLCollisionBox& b = colliders[handle];
					if (collides(a, b)) {
						a.collisions.insert(handle);
					}
				}
			}
		}
	}

	void init(
		fl_handle handle,
		fl_handle parent,
		float x,
		float y,
		float w,
		float h
	) {
		colliders[handle].handle = handle;
		colliders[handle].parent = parent;
		colliders[handle].x = x;
		colliders[handle].y = y;
		colliders[handle].w = w;
		colliders[handle].h = h;

		colliders[handle].collisions.clear();
		colliders[handle].target_groups.clear();
	}

	fl_handle create(
		fl_handle parent,
		float x,
		float y,
		float w,
		float h
	) {
		fl_handle handle = colliders.create();

		if (handle != NULL_HANDLE) {
			init(handle, parent, x, y, w, h);
		} else {
			std::cout << "Warning: could not create collider.\n";
		}
		
		return handle;
	}

	fl_handle create(fl_handle parent, float w, float h) {
		return create(parent, 0.f, 0.f, w, h);
	}

	void destroy(fl_handle handle) {
		for (auto [name, group] : groups) {
			group.erase(handle);
		}
		colliders.destroy(handle);
	}

	void add_to_group(fl_handle handle, std::string group) {
		if (handle != NULL_HANDLE) {
			if (groups.find(group) == groups.end()) {
				groups[group] = std::unordered_set<fl_handle>();
			}

			groups[group].insert(handle);
		}
	}

	void add_target_group(fl_handle handle, std::string group) {
		if (handle != NULL_HANDLE) {
			if (groups.find(group) == groups.end()) {
				groups[group] = std::unordered_set<fl_handle>();
			}

			colliders[handle].target_groups.insert(group);
		}
	}
}

