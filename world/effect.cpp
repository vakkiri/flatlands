/*
 * 	effect.cpp
 *
 */

#include <iostream>
#include "effect.h"

#include "common/fl_object.h"
#include "components/animator/fl_animator.h"


namespace FLEffects {

	void destroy(fl_handle handle) {
		FLObjects::destroy(handle);
	}

	void update(FLObject& effect) {
		if (FLAnimators::finished(effect.animators["animator"])) {
			destroy(effect.handle);
		}
	}

	void create(float x, float y, std::string collection, unsigned int ticks_per_frame) {
		fl_handle handle = FLObjects::create();

		if (handle != NULL_HANDLE) {
			FLObjects::set_pos(handle, x, y);
			FLObjects::add_texture(handle, "texture", "world", collection);
			FLObjects::add_animator(handle, "animator", "texture", collection, ticks_per_frame, false);
			FLObjects::add_script(handle, update);
		} else {
			std::cout << "Warning: could not create effect.\n";
		}
	}
}

