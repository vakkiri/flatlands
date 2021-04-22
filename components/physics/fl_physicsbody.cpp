/*
 * 	fl_physicsbody.cpp
 *
 */

#include "common/fl_object.h"
#include "common/fl_static_buffer.h"
#include "components/components.h"

#define INITIAL_BODIES 5000

namespace FLPhysicsBodies {
	FLStaticBuffer<FLPhysicsBody> bodies(INITIAL_BODIES);

	void update(FLPhysicsBody& body) {
		FLObject* parent = FLObjects::get(body.parent);

		parent->x += body.vx;
		parent->y += body.vy;
		
		// TODO gravity, friction, tilemap, collisions
		body.touched_tilemap = FLColliders::touches_tilemap(body.collider);
	}

	void update() {
		for (FLPhysicsBody& body : bodies) {
			update(body);
		}
	}

	void init(
		fl_handle handle,
		fl_handle parent,
		fl_handle collider,
		float mass,
		float gravity_scale
	) {
		bodies[handle].parent = parent;
		bodies[handle].collider = collider;
		bodies[handle].mass = mass;
		bodies[handle].gravity_scale = gravity_scale;
		bodies[handle].vx = 0.f;
		bodies[handle].vy = 0.f;
		bodies[handle].touched_tilemap = false;
	}

	void destroy(fl_handle handle) {
		bodies.destroy(handle);
	}

	fl_handle create(fl_handle parent, fl_handle collider) {
		fl_handle handle = bodies.create();

		if (handle != NULL_HANDLE) {
			init(handle, parent, collider, 0.f, 1.f);
		}

		return handle;
	}

	void accelerate(fl_handle handle, float x, float y) {
		bodies[handle].vx += x;
		bodies[handle].vy += y;
	}

	bool touched_tilemap(fl_handle handle) {
		return bodies[handle].touched_tilemap;
	}
}

