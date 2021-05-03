/*
 * 	fl_colliders.cpp
 *
 */

#include <algorithm>
#include <iostream>
#include <utility>
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
		float ax = a.x + FLObjects::x(a.parent);
		float ay = a.y + FLObjects::y(a.parent);
		float aw = a.w;
		float ah = a.h;
		float bx = b.x + FLObjects::x(b.parent);
		float by = b.y + FLObjects::y(b.parent);
		float bw = b.w;
		float bh = b.h;

		return rect_collision(ax, ay, aw, ah, bx, by, bw, bh);
	}

	float x(fl_handle handle) {
		return colliders[handle].x + FLObjects::x(colliders[handle].parent);
	}

	float y(fl_handle handle) {
		return colliders[handle].y + FLObjects::y(colliders[handle].parent);
	}
	bool touches_tilemap(fl_handle handle) {
		// TODO: this alg could be refactored into collision utils
		float endx = x(handle) + colliders[handle].w;
		float endy = y(handle) + colliders[handle].h;
		FLTilemap* tilemap = FLGame::instance().environment()->tilemap();
	
		// TODO: 16 should be based on tile size and not hardcoded	
		for (float startx = x(handle); startx <= endx; startx += 16) {
			for (float starty = y(handle); starty <= endy; starty += 16) {
				float _x = std::min(startx, endx);
				float _y = std::min(starty, endy);

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
			a.collisions.clear();
			for (auto group_name : a.target_groups) {
				for (auto handle : groups[group_name]) {
					FLCollisionBox& b = colliders[handle];
					if (collides(a, b)) {
						fl_handle body = colliders[handle].parent;
						a.collisions.insert(body);
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
		for (std::pair<const std::string, std::unordered_set<fl_handle>>& group : groups) {
			group.second.erase(handle);
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

	FLCollisionBox* get(fl_handle handle) {
		return &colliders[handle];
	}
}

