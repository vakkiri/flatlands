/*
 * 	fl_colliders.cpp
 *
 */

#include <iostream>
#include "fl_colliders.h"
#include "common/fl_static_buffer.h"

#define INITIAL_COLLIDERS 5000

namespace FLColliders {
	FLStaticBuffer<FLCollisionBox> colliders(INITIAL_COLLIDERS);
	std::unordered_map<std::string, std::unordered_set<fl_handle>> groups;

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

