/*
 * 	fl_physics_handler.h
 *
 * 	Component for handling an object's physics interactions.
 *
 */

#ifndef FL_PHYSICS_H_
#define FL_PHYSICS_H_

#include "../../common/common.h"

class FLGameObject;

class FLPhysicsHandler {
	public:
		FLPhysicsHandler();
		bool init(FLGameObject* owner, std::string collider_name);

		void update();
		void accelerate( float x, float y);
		void stop();

		float xvel();
		float yvel();

		bool on_ground();
		bool alive();
		void kill();
	protected:
		void move();
		void apply_gravity();
		void apply_friction();


		FLGameObject* owner;
		std::string collider_name;

		point vel;
		float gravity_factor;
		int on_ground_timer;
		bool _alive;
};

#endif

