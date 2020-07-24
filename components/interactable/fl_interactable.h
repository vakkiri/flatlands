/*
 * 	fl_interactable.h
 *
 * 	Objects that respond to the player's "action" key
 *
 */

#ifndef FL_INTERACTABLE_H_
#define FL_INTERACTABLE_H_

class FLGameObject;

class FLInteractable {
	public:
		FLInteractable();

		void interact();
	protected:
		FLGameObject *owner;

};

#endif

