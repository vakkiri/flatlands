/*
 * 	physics_object.h
 *
 * 	Objects which move and are affected by world physics.
 *
 */

#ifndef PHYSICS_OBJECT_H_
#define PHYSICS_OBJECT_H_

#include "world_object.h"

class FLPhysics;
class FLWorldEnvironment;

class FLPhysicsObject : virtual public FLWorldObject {
	public:
		FLPhysicsObject();
		FLPhysicsObject( float x, float y, float w, float h );

		float bounds_x();
		float bounds_y();
		float bounds_w();
		float bounds_h();

		void set_bounds_margin( rect& new_bounds );

		bool collides_with( FLPhysicsObject& other );
		bool collides_with_tile();

		virtual void update_position();
		virtual void update_physics();

		void set_vel(point vel);
		void move( point amt );
		void accelerate( point amt );

		void stop_horizontal();
		void stop_vertical();

	private:
		FLPhysics& physics;
		FLWorldEnvironment& environment;

		point vel;
		point accel;
		rect bounds_margin;

};

#endif

