/*
 * 	scenery.cpp
 *
 */

#include <vector>
#include <iostream>

#include "common/basic_types.h"
#include "scenery.h"
#include "rendering/renderer.h"
#include "resources/fl_collections.h"

#define START_INDEX 101

namespace FLScenery {
	std::vector<fl_handle> scenery;

	void init(fl_handle handle, float x, float y, int type) {
		int index = type - START_INDEX;

		FLCollection collection = FLCollections::get("scenery");
		
		if (index < 0 || index >= (int) collection.num_elements) {
			std::cout << "Invalid scenery: " << index << std::endl;
			return;
		}

		FLCollectionElement e = collection.elements[index];
		FLObjects::set_pos(handle, x, y);
		FLObjects::add_texture(handle, "", "scenery", e.s, e.t, e.w, e.h);
	}

	void create(float x, float y, int type) {
		fl_handle new_scenery = FLObjects::create();

		if (new_scenery != NULL_HANDLE) {
			init(new_scenery, x, y, type);
		} else {
			std::cout << "Warning: could not create scenery.\n";
		}
	}

	void clear() {
		while (!scenery.empty()) {
			FLObjects::destroy(scenery.back());
			scenery.pop_back();
		}
	}
}
