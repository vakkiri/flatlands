/*
 * 	fl_physics_handler_manager.cpp
 *
 */

#include "fl_physics_handler_manager.h"
#include "../components.h"

const int NUM_PHYSICS_HANDLERS = 1000;

FLPhysicsHandler physics_handlers[NUM_PHYSICS_HANDLERS];

static int next_free = 0;
static int slots_used = 0;

int new_physics_handler(FLGameObject *owner, std::string collider_name) {
	int handle = -1;
	int slots_checked = 0;

	if (slots_used < NUM_PHYSICS_HANDLERS) {

		// The slots_checked counter should not be necessary, but I really hate
		// infinite loops even if they should be impossible...
		while (physics_handlers[next_free].alive() &&
			   slots_checked < NUM_PHYSICS_HANDLERS) {
			next_free++;
			slots_checked++;
			if (next_free >= NUM_PHYSICS_HANDLERS) {
				next_free = 0;
			}
		}

		// if ( !physics_handlers[next_free].alive() )
		handle = next_free;

		if (physics_handlers[handle].init(owner, collider_name)) {
			slots_used++;
		} else {
			handle = -1;
		}
	}

	return handle;
}

void delete_physics_handler(int handle) {
	if (handle >= 0 && handle < NUM_PHYSICS_HANDLERS &&
		physics_handlers[handle].alive()) {
		physics_handlers[handle].kill();
		slots_used--;
	}
}

void update_physics_handlers() {
	for (int i = 0; i < NUM_PHYSICS_HANDLERS; ++i) {
		if (physics_handlers[i].alive()) {
			physics_handlers[i].update();
		}
	}
}

FLPhysicsHandler *get_physics_handler(int handle) {
	FLPhysicsHandler *physics_handler = nullptr;
	if (handle >= 0 && handle < NUM_PHYSICS_HANDLERS &&
		physics_handlers[handle].alive()) {
		physics_handler = &(physics_handlers[handle]);
	}

	return physics_handler;
}
