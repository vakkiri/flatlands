/*
 * 	fl_interactable.cpp
 *
 */

#include <iostream>
#include "fl_interactable.h"
#include "../../common/game_object.h"

FLInteractable::FLInteractable() {
	owner = nullptr;
}

void FLInteractable::interact() {
	if (owner != nullptr) {
		owner->interact();
	} else {
		std::cout << "Warning: tried to interact with null owner." << std::endl;
	}
}
