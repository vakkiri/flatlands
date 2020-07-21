/*
 * 	fl_collider_manager.cpp
 *
 */

#include "fl_collider_manager.h"
#include "../components.h"
#include "../../utils/collision_utils.h"

#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

const int NUM_COLLIDERS = 1000;

FLCollider colliders[NUM_COLLIDERS];
std::unordered_map<std::string, std::unordered_set<int>> collider_groups;
std::unordered_set<int> empty_set;

static int next_free = 0;
static int slots_used = 0;

int fl_new_collider(FLGameObject *owner, std::string shape) {
	int handle = -1;
	int slots_checked = 0;

	if (slots_used < NUM_COLLIDERS) {

		// The slots_checked counter should not be necessary, but I really hate
		// infinite loops even if they should be impossible...
		while (colliders[next_free].alive() && slots_checked < NUM_COLLIDERS) {
			next_free++;
			slots_checked++;
			if (next_free >= NUM_COLLIDERS) {
				next_free = 0;
			}
		}

		handle = next_free;

		if (colliders[handle].init(owner, shape)) {
			slots_used++;
		} else {
			std::cout << "Error creading collider handle: could not initialize\n";
			handle = -1;
		}
	}

	return handle;
}

void fl_delete_collider(int handle) {
	if (handle >= 0 && handle < NUM_COLLIDERS && colliders[handle].alive()) {
		colliders[handle].kill();
		for (auto group : colliders[handle].get_collision_groups()) {
			fl_remove_collider_from_group(handle, group);
		}
		slots_used--;
	}
}

void fl_update_colliders() {
	for (int i = 0; i < NUM_COLLIDERS; ++i) {
		if (colliders[i].alive()) {
			colliders[i].update();
		}
	}
}

void fl_add_collider_to_group(int handle, std::string group) {
	if (colliders[handle].alive()) {
		colliders[handle].add_collision_group(group);

		if (collider_groups.find(group) == collider_groups.end()) {
			collider_groups[group] = std::unordered_set<int>();
		}

		collider_groups[group].insert(handle);
	}
}

void fl_remove_collider_from_group(int handle, std::string group) {
	auto grp = collider_groups.find(group);
	if (grp != collider_groups.end()) {
		auto hndl = grp->second.find(handle);
		if (hndl != grp->second.end()) {
			grp->second.erase(handle);
		}
	}
}

FLCollider *fl_get_collider(int handle) {
	FLCollider *collider = nullptr;
	if (handle >= 0 && handle < NUM_COLLIDERS && colliders[handle].alive()) {
		collider = &(colliders[handle]);
	}

	return collider;
}

std::unordered_set<int> &fl_get_group_colliders(std::string group) {
	auto it = collider_groups.find(group);
	if (it != collider_groups.end()) {
		return it->second;
	} else {
		return empty_set;
	}
}

std::pair<point, FLGameObject*> get_nearest_collision(fl_line* line, std::unordered_set<std::string> groups) {
	point p = point {-1, -1};
	point intersect;
	std::vector<fl_line> edges = std::vector<fl_line>(2);

	FLGameObject* obj = nullptr;

	for (auto group : groups) {
		for (int i : fl_get_group_colliders(group)) {
			if (colliders[i].alive()) {
				FLShape* shape = colliders[i].get_shape();
				// instead of checking each outside edge, I'm just putting an X through the rect
				// for monsters etc. this will make it look like the inside got hit
				edges[0].u.x = shape->x();
				edges[0].u.y = shape->y();
				edges[0].v.x = shape->w();
				edges[0].v.y = shape->h();

				edges[1].u.x = shape->x();
				edges[1].u.y = shape->y() + shape->h();
				edges[1].v.x = shape->w();
				edges[1].v.y = 0;

				for (fl_line& edge : edges) {
					intersect = intersection(line, &edge);

					if (intersect.x >= 0 && intersect.y >= 0) {
						// if this intersection is closer
						// this doesn't work lmao i need to take an actual distance
						// along the line wtf was i thinking
						if (p.x == -1 || (intersect.x - line->u.x < p.x - line->u.x)) {
							p.x = intersect.x;
							p.y = intersect.y;
							obj = colliders[i].get_owner();
						}
					}
				}
			}
		}
	}

	return std::make_pair(p, obj);
}

