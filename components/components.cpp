/*
 * 	components.cpp
 *
 */

#include "components.h"

void fl_update_components() {
	// TODO: these should all follow the fl_update_xxx naming convention
	update_updators();
	update_physics_handlers();
	fl_update_colliders();
}
