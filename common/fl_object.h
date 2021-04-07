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
#include "components/components.h"

class FLObject {
	public:
		FLObject();

		void destroy();

		void add_shape(std::string name, float x, float y, float w, float h);
		void add_texture(std::string name, std::string image, float x, float y, float w, float h, float s, float t);

		FLShape* shape(std::string name);
		FLTexture* texture(std::string name);

	private:
		std::unordered_map<std::string, fl_state> states;
		std::unordered_map<std::string, FLShape*> shapes;
		std::unordered_map<std::string, FLTexture*> textures;
		std::function<void()> update;
};

namespace FLObjects {
	FLObject* create();
}

#endif

