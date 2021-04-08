/*
 * 	scenery.cpp
 *
 */

#include <vector>
#include <iostream>

#include "common/basic_types.h"
#include "scenery.h"
#include "../rendering/renderer.h"

namespace FLScenery {
	std::vector<fl_handle> scenery;

	void init(fl_handle handle, float x, float y, int type) {
		// TODO: LOAD FROM FILE LOL RIGHT NOW B4 I DO ANYTHING ELSE
		float s, t, w, h;

		if (type == 101) {
			s = 0;
			t = 0;
			w = 98;
			h = 88;
		} else if (type == 102) {
			s = 99;
			t = 0;
			w = 35;
			h = 27;
		} else if (type == 103) {
			s = 0;
			t = 96;
			w = 245;
			h = 68;
		} else if (type == 104) {
			s = 144;
			t = 0;
			w = 32;
			h = 71;
		} else if (type == 105) {
			s = 256;
			t = 0;
			w = 51;
			h = 98;
		} else if (type == 106) {
			s = 320;
			t = 0;
			w = 48;
			h = 64;
		} else if (type == 107) {
			s = 368;
			t = 0;
			w = 53;
			h = 70;
		} else if (type == 108) {
			s = 176;
			t = 0;
			w = 32;
			h = 81;
		} else {
            		return;
		}

		FLObjects::set_pos(handle, x, y);
		FLObjects::add_texture(handle, "", "scenery", s, t, w, h);
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
