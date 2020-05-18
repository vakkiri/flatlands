/*
 * 	fl_update_manager.cpp
 *
 */

#include <iostream>

#include "fl_update_manager.h"
#include "../../common/common.h"

#define NUM_UPDATE_HANDLERS update_handlers.size()

const int DEFAULT_UPDATE_HANDLERS = 1000;

std::vector<FLGameObject*> update_handlers = std::vector<FLGameObject*>( DEFAULT_UPDATE_HANDLERS, nullptr );

static int next_free = 0;
static int slots_used = 0;

int new_updator( FLGameObject* obj ) {
	int handle = -1;
	int slots_checked = 0;

	if ( obj == nullptr ) {
		std::cout << "Error: Can't create updator for null object.\n";
	}
	else {
		if ( slots_used >= NUM_UPDATE_HANDLERS ) {
			update_handlers.resize( NUM_UPDATE_HANDLERS * 2, nullptr );
		}

		// slots checked should not be needed, but I like to be sure we won't have an infinite loop...
		while ( update_handlers[next_free] != nullptr && slots_checked < NUM_UPDATE_HANDLERS ) {
			next_free++;
			slots_checked++;
			if ( next_free >= NUM_UPDATE_HANDLERS ) {
				next_free = 0;
			}
		}

		if ( update_handlers[next_free] == nullptr ) {
			handle = next_free;
			update_handlers[handle] = obj;
			slots_used++;
		}
	}
	
	return handle;
}

void delete_updator( int handle ) {
	if ( handle >= 0 && handle < NUM_UPDATE_HANDLERS ) {
		update_handlers[handle] = nullptr;
		slots_used--;
	}
}

void update_updators() {
	for ( FLGameObject* obj : update_handlers ) {
		if ( obj != nullptr ) {
			obj->per_frame_update();
		}
	}
}
