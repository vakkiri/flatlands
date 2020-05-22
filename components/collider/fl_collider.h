/*
 * 	fl_collider.h
 *
 * 	Collision component.
 *
 */

#ifndef FL_COLLIDER_H_
#define FL_COLLIDER_H_

#include <functional>
#include <unordered_set>
#include <string>
#include <vector>

class FLGameObject;
class FLShape;
class FLTilemap;

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
		void detect_collisions();
		void process_collisions();
		void add_collision( FLCollider* collision );

		void add_collision_group( std::string group );
		void add_target_collision_group( std::string group );

		std::unordered_set<std::string>& get_collision_groups();

		FLShape* get_shape();

		void set_collision_method( std::function<void(FLCollider*)> meth );
	protected:
		FLGameObject* owner;
		std::unordered_set<std::string> target_collision_groups;
		std::unordered_set<std::string> collision_groups;
		std::string shape_name;
		std::vector<FLCollider*> collisions;
		std::function<void(FLCollider*)> on_collision;

		bool _alive;
		bool has_collision_method;
};

#endif

