/*
 * 	fl_physicsbody.cpp
 *
 */

#include <unordered_map>
#include "common/fl_object.h"
#include "common/fl_static_buffer.h"
#include "components/components.h"

#define INITIAL_BODIES 5000

namespace FLPhysicsBodies {
	FLStaticBuffer<FLPhysicsBody> bodies(INITIAL_BODIES);
	std::unordered_map<std::string, float> globals;

	void set_global(std::string name, float value) {
		globals[name] = value;
	}

	void apply_gravity(FLPhysicsBody& body) {
		body.vy += globals["gravity"] * body.gravity_scale;
	}

	void update(FLPhysicsBody& body) {
		FLObject* parent = FLObjects::get(body.parent);
		float original_x = parent->x;
		float original_y = parent->y;
		
		body.right_touched_wall = false;
		body.left_touched_wall = false;
		body.top_touched_wall = false;
		body.bottom_touched_wall = false;

		apply_gravity(body);

		parent->x += body.vx;
		parent->y += body.vy;

		// TODO: account for potential collisions which are "jumped over"

		while (FLColliders::touches_tilemap(body.collider)) {
			if (body.vx > 0 && parent->x > original_x) {
				parent->x -= 1;
			} else if (body.vx < 0 && parent->x < original_x) {
				parent->x += 1;
			}
			if (body.vy > 0 && parent->y > original_y) {
				parent->y -= 1;
			} else if (body.vy < 0 && parent->y < original_y) {
				parent->y += 1;
			}
		}

		// Check if we should stop due to running into a side
		if (body.vx > 0) {
			parent->x += 1;
			if (FLColliders::touches_tilemap(body.collider)) {
				body.vx = 0;
				body.right_touched_wall = true;
			}
			parent->x -= 1;
		} else if (body.vx < 0) {
			parent->x -= 1;
			if (FLColliders::touches_tilemap(body.collider)) {
				body.vx = 0;
				body.left_touched_wall = true;
			}
			parent->x += 1;
		}
		if (body.vy > 0) {
			parent->y += 1;
			if (FLColliders::touches_tilemap(body.collider)) {
				body.vy = 0;
				body.bottom_touched_wall = true;
			}
			parent->y -= 1;
		} else if (body.vy < 0) {
			parent->y -= 1;
			if (FLColliders::touches_tilemap(body.collider)) {
				body.vy = 0;
				body.top_touched_wall = true;
			}
			parent->y += 1;
		}


		
		// TODO gravity, friction, tilemap, collisions
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
		bodies[handle].right_touched_wall = false;
		bodies[handle].left_touched_wall = false;
		bodies[handle].top_touched_wall = false;
		bodies[handle].bottom_touched_wall = false;
	}

	void destroy(fl_handle handle) {
		bodies.destroy(handle);
	}

	fl_handle create(
		fl_handle parent,
		fl_handle collider,
		float gravity_scale ) {
		fl_handle handle = bodies.create();

		if (handle != NULL_HANDLE) {
			init(handle, parent, collider, 0.f, gravity_scale);
		}

		return handle;
	}

	fl_handle create(fl_handle parent, fl_handle collider) {
		return create(parent, collider, 1.0);
	}

	void accelerate(fl_handle handle, float x, float y) {
		bodies[handle].vx += x;
		bodies[handle].vy += y;
	}

	bool touched_tilemap(fl_handle handle) {
		return (
			bodies[handle].right_touched_wall	||
			bodies[handle].left_touched_wall 	||
			bodies[handle].top_touched_wall 	||
			bodies[handle].bottom_touched_wall
		);
	}
}

