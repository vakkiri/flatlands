/*
 * 	fl_game_object.cpp
 *
 */

#include <iostream>
#include "fl_static_buffer.h"
#include "fl_object.h"
#include "components/components.h"

#define DEFAULT_NUM_OBJECTS 10000

namespace FLObjects {
	FLStaticBuffer<FLObject> objects(DEFAULT_NUM_OBJECTS);

	void pass() {} // pass function for objects with no update

	fl_handle create(fl_handle parent) {
		fl_handle handle = objects.create();

		// Resetting attributes is done lazilly, so it is here
		// instead of destroy
		if (handle != NULL_HANDLE) {
			objects[handle].handle = handle;
			objects[handle].parent = parent;
			objects[handle].x = 0;
			objects[handle].y = 0;
			objects[handle].states.clear();
			objects[handle].textures.clear();
			objects[handle].update = pass;
		}

		return handle;
	}

	fl_handle create() {
		return create(NULL_HANDLE);
	}

	FLObject* get(fl_handle handle) {
		if (handle != NULL_HANDLE) {
			return &objects[handle];
		} else {
			std::cout << "Tried to access null object handle.\n";
			return nullptr;
		}
	}

	void destroy(fl_handle handle) {
		// TODO: should we keep track of children, so they
		// can be destroyed as well?
		for (auto [name, texture] : objects[handle].textures) {
			FLTextures::destroy(texture);
		}

		objects.destroy(handle);
	}

	void update() {
		for (auto obj : objects) {
			obj.update();
		}
	}

	void render() {
		for (auto obj : objects) {
			for (auto [name, handle] : obj.textures) {
				FLTextures::render(handle);
			}
		}
	}

	void set_pos(fl_handle handle, float x, float y) {
		if (handle != NULL_HANDLE) {
			objects[handle].x = x;
			objects[handle].y = y;
		} else {
			std::cout << "Error: tried to position of object with null handle.";
		}
	}

        void add_texture(
                fl_handle handle,
		std::string name,
                std::string surface,
                float s,
                float t,
		float w,
		float h,
		bool reversed
        ) {
		fl_handle texture = FLTextures::create(surface, handle, s, t, w, h, reversed);

		objects[handle].textures[name] = texture;
	}

        void add_texture(
                fl_handle handle,
		std::string name,
                std::string surface,
                float s,
                float t,
		float w,
		float h
        ) {
		add_texture(handle, name, surface, s, t, w, h, false);
	}

	float x(fl_handle handle) {
		float x = objects[handle].x;

		if (objects[handle].parent != NULL_HANDLE) {
			x += objects[objects[handle].parent].x;
		}

		return x;
	}

	float y(fl_handle handle) {
		float y = objects[handle].y;

		if (objects[handle].parent != NULL_HANDLE) {
			y += objects[objects[handle].parent].y;
		}

		return y;
	}

}
