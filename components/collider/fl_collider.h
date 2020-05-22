/*
 * 	fl_collider.h
 *
 * 	Collision component.
 *
 */

#ifndef FL_COLLIDER_H_
#define FL_COLLIDER_H_

#include <unordered_set>
#include <string>
#include <vector>

class FLGameObject;
class FLShape;
class FLTilemap;

struct fl_collision {
	std::unordered_set<std::string> collision_groups;
	FLGameObject* colliding_object;
};

class FLCollider {
	public:
		FLCollider();
		bool init(FLGameObject* owner, std::string shape_name, std::string name);

		bool touches_tilemap();
		bool bottom_touches_tilemap();
		bool top_touches_tilemap();
		bool left_touches_tilemap();
		bool right_touches_tilemap();

		bool alive();
		void kill();

		void update();
		void add_collision_group( std::string group );
		void add_target_collision_group( std::string group );

		std::unordered_set<std::string>& get_collision_groups();

		fl_collision& pop_collision();

		FLShape* get_shape();
	protected:
		FLGameObject* owner;
		std::unordered_set<std::string> target_collision_groups;
		std::unordered_set<std::string> collision_groups;
		std::string shape_name;
		std::vector<fl_collision> collisions;

		bool _alive;
};

#endif

