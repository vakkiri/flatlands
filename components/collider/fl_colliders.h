/*
 * 	fl_colliders.h
 *
 * 	Component for collision areas.
 *
 */

#ifndef FL_COLLIDERS_H_
#define FL_COLLIDERS_H_

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "common/basic_types.h"
#include "common/fl_object.h"

struct FLCollisionBox {
	fl_handle handle;
	fl_handle parent;
	float x;
	float y;
	float w;
	float h;
	std::unordered_set<fl_handle> collisions;
	std::unordered_set<std::string> object_groups;
	std::unordered_set<std::string> target_groups;
};

namespace FLColliders {
	void add_to_group(fl_handle handle, std::string group);
	void add_target_group(fl_handle handle, std::string group);
	void destroy(fl_handle handle);

	fl_handle create(fl_handle parent, float x, float y, float w, float h);
	fl_handle create(fl_handle parent, float w, float h);

	FLCollisionBox* get(fl_handle handle);
}

#endif
