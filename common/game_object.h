/*
 * 	game_object.h
 *
 * 	Base type of bounded game objects.
 *
 */

#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <string>
#include <unordered_map>

#include "basic_types.h"

class FLCollider;
class FLEnvironment;
class FLGame;
class FLShape;
class FLPhysicsHandler;

class FLGameObject {
	public:
		FLGameObject();
		FLGameObject(float x, float y, float w, float h);
		~FLGameObject();

		virtual void set_x( float x );
		virtual void set_y( float y );
		virtual float x();
		virtual float y();
		virtual float w();
		virtual float h();

		void movex( float x );
		void movey( float y );
		void move( float x, float y);
		void move( point amt );

		virtual void per_frame_update() {};

		FLShape* get_shape( std::string name );
		FLCollider* get_collider( std::string name );

		void add_collider( std::string shape, std::string name );
		bool is_active();
		
		FLPhysicsHandler* physics_handler();

		FLEnvironment* environment();

	protected:
		std::unordered_map<std::string, FLShape*> shapes;
		std::unordered_map<std::string, FLCollider*> colliders;
		int physics_handler_handle;
		int updator_handle;
};

#endif
