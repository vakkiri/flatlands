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
#include <variant>

#include "common/basic_types.h"

struct FLObject;

typedef std::variant<int,float> fl_var;
typedef std::function<void(FLObject&)> fl_script;

struct FLObject {
	// TODO: keep track of children via a map?
	fl_handle handle;
	fl_handle parent;
	float x;
	float y;
	std::unordered_map<std::string, fl_state> states;
	std::unordered_map<std::string, fl_handle> textures;
	std::unordered_map<std::string, fl_handle> animators;
	std::unordered_map<std::string, fl_var> vars;
	std::vector<fl_script> scripts;
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

	void add_script(fl_handle handle, std::function<void(FLObject&)> f);

	void add_var(
		fl_handle handle,
		std::string name,
		fl_var value
	);

	void add_state(
		fl_handle handle,
		std::string name,
		fl_state value
	);

	void set_texture(
		fl_handle handle,
		std::string name,
		std::string collection,
		int index
	);

	void set_texture(
		fl_handle handle,
		std::string name,
		std::string collection
	);

	void add_texture(
		fl_handle handle,
		std::string name,
		std::string surface,
		std::string collection
	);

	void add_texture(
		fl_handle handle,
		std::string name,
		std::string surface,
		std::string collection,
		int index
	);

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

	void set_animation(
		fl_handle handle,
		std::string name,
		std::string collection
	);

	void add_animator(
		fl_handle handle,
		std::string name,
		std::string texture,
		std::string collection,
		unsigned int ticks_per_frame
	);

	void add_animator(
		fl_handle handle,
		std::string name,
		std::string texture,
		std::string collection,
		unsigned int ticks_per_frame,
		bool repeats
	);

	// member access
	float x(fl_handle handle);
	float y(fl_handle handle);
}

#endif

