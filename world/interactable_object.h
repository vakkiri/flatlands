/*
 * 	interactable_object.h
 *
 * 	Objects which the player can interact with via a button press.
 * 	For example, npcs.
 *
 */

#ifndef INTERACTABLE_OBJECT_H_
#define INTERACTABLE_OBJECT_H_

#include "world_object.h"

class FLInteractableObject : virtual public FLWorldObject {
	public:
		virtual ~FLInteractableObject(){}
		virtual void interact_with() = 0;
};

#endif

