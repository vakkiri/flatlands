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

#include "fl_accessor.h"
#include "../components/components.h"

class FLObject {
	public:
		FLObject();

		void destroy();

		//void add_child(FLAccessor child);
		void add_shape(std::string name, float x, float y, float w, float h);
		void add_texture(std::string name, std::string image, float x, float y, float w, float h, float s, float t);
		/*
		void add_animation(...);
		void add_collider();
		void add_physics();
		*/

		FLAccessor<FLShape> shape(std::string name);
		/*
		FLAccessor<FLTexture> texture(std::string name);
		FLAccessor<FLAnimation> animation(std::string name);
		FLAccessor<FLCollider> collider(std::string name);
		FLAccessor<FLPhysics> physics(std::string name);
		*/
	private:
		//std::unordered_map<std:string, FLAccessor<FLObject>> children;
		std::unordered_map<std::string, FLAccessor<FLShape>> shapes;
		std::function<void()> update; // TODO: accept a timedelta?

};

#endif

