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

bool FLInteractable::alive() {
	return owner != nullptr;
}

void FLInteractable::kill() {
	owner = nullptr;
}

void FLInteractable::set_owner(FLGameObject* owner) {
	if (this->owner != nullptr ) {
		std::cout << "Warning: replacing existing non-null interactable owner." << std::endl;
	}
	this->owner = owner;
}

FLGameObject* FLInteractable::get_owner() {
	return owner;
}

