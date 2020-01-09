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
		
		bool on_ground();
	protected:
		unsigned int on_ground_timer;

		virtual void apply_gravity();
		virtual void apply_friction();

		point vel;
		point accel;
		FLPhysics& physics;
		FLWorldEnvironment& environment;
	private:

		rect bounds_margin;

};

#endif

