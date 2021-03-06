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
	bool init(FLGameObject *owner, std::string collider_name);

	void update();
	void accelerate(float x, float y);
	void stop();
	void stopx();
	void stopy();

	float xvel();
	float yvel();

	bool on_ground();
	bool on_wall();
	bool alive();
	void kill();

	void set_gravity_factor(float factor);
	void set_friction_factor(float factor);
	void unbound_velocity();

  protected:
	void move();
	void apply_gravity();
	void apply_friction();
	void bound_velocity();

	FLGameObject *owner;
	std::string collider_name;

	point vel;
	float gravity_factor;
	float friction_factor;
	int on_ground_timer;
	int on_wall_timer;
	bool bounded_velocity;
	bool _alive;
};

#endif
