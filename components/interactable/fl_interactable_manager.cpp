/*
 * 	fl_interactable_manager.cpp
 *
 */

#include <iostream>
#include <vector>
#include "fl_interactable_manager.h"
#include "fl_interactable.h"
#include "../../common/game_object.h"
#include "../../utils/collision_utils.h"

static std::vector<FLInteractable> interactables = std::vector<FLInteractable>(500);
int last_free = 0;

int fl_new_interactable(FLGameObject *owner) {
	int handle = -1;
	// find the next free slot if one exists
	if (interactables[last_free].alive()) {
		int end = last_free;
		last_free += 1;
		while (last_free != end) {
			// if we find a free space
			if (!interactables[last_free].alive()) {
				break;
			} else {
				last_free += 1;
				// note: this will break if code is added which resizes 
				// interactables past max int
				if (last_free >= (int) interactables.size()) {
					last_free = 0;
				}
			}
		}
	}

	// this check covers potentially having all interactables full
	// could keep a count of how many interactables we have an resize instead
	if (!interactables[last_free].alive()) {
		handle = last_free;
		interactables[last_free].set_owner(owner);
	} else {
		std::cout << "Warning: all interactable spots full." << std::endl;
	}

	return handle;
}

void fl_delete_interactable(int handle) {
	if (handle < 0 || handle >= (int) interactables.size()) {
		std::cout << "Warning: tried to delete interactable at invalid position." << std::endl;
	} else {
		if (interactables[handle].alive()) {
			interactables[handle].kill();
		} else {
			std::cout << "Warning: attempted double kill of interactable." << std::endl;
		}
	}
}

void fl_interact(FLGameObject* obj) {
	(void) obj;
	for (auto i : interactables) {
		if (i.alive()) {
			// TODO: should support setting a specific shape or collider
			if (rect_collision(i.get_owner()->get_shape("position"), obj->get_shape("position"))) {
				i.interact();
				break;
			}
		}
	}
}

