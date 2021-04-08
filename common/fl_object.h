/*
 * 	fl_game_object.h
 *
 * 	Updated game object class, for use with updated component system.
 *
 */

#ifndef FL_GAME_OBJECT_H_
#define FL_GAME_OBJECT_H_

#include <functional>
#include <string>
#include <unordered_map>

#include "common/basic_types.h"

struct FLObject {
	// TODO: keep track of children via a map?
	fl_handle handle;
	fl_handle parent;
	float x;
	float y;
	std::unordered_map<std::string, fl_state> states;
	std::unordered_map<std::string, fl_handle> textures;
	std::function<void()> update;
};

namespace FLObjects {
	fl_handle create();
	fl_handle create(fl_handle parent);
	FLObject* get(fl_handle handle);
	void destroy(fl_handle handle);
	void update();
	void render();

	// member modification
	void set_pos(fl_handle handle, float x, float y);

	void add_texture(
		fl_handle handle,
		std::string name,
		std::string surface,
		float s,
		float t,
		float w,
		float h,
		bool reversed
	);

	void add_texture(
		fl_handle handle,
		std::string name,
		std::string surface,
		float s,
		float t,
		float w,
		float h
	);

	// member access
	float x(fl_handle handle);
	float y(fl_handle handle);
}

#endif

