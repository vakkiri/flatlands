/*
 * 	fl_physicsbody.h
 *
 * 	Moving bodies which collide with the environment.
 *
 */

#ifndef FL_PHYSICSBODY_H_
#define FL_PHYSICSBODY_H_

#include "common/basic_types.h"

struct FLPhysicsBody {
	fl_handle parent;
	fl_handle collider;
	float vx;
	float vy;
	float mass;
	float gravity_scale;
};

namespace FLPhysicsBodies {
	void update();

	void accelerate(fl_handle handle, float x, float y);

	void destroy(fl_handle handle);
	fl_handle create(fl_handle parent, fl_handle collider);
}

#endif

