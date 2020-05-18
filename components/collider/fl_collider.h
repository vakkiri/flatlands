/*
 * 	fl_collider.h
 *
 * 	Collision component.
 *
 */

#ifndef FL_COLLIDER_H_
#define FL_COLLIDER_H_

#include <string>

class FLGameObject;
class FLTilemap;

class FLCollider {
	public:
		FLCollider() = delete;
		FLCollider(FLGameObject* owner, std::string shape_name);

		bool touches_tilemap();
		bool bottom_touches_tilemap();
		bool top_touches_tilemap();
		bool left_touches_tilemap();
		bool right_touches_tilemap();
	protected:
		FLGameObject* owner;
		std::string shape_name;
		std::string name;
};

#endif

