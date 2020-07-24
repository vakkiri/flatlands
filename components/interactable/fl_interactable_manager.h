/*
 * 	fl_interactable_manager.h
 *
 * 	manage creation/deletion etc. of interactables
 *
 */

#ifndef FL_INTERACTABLE_MANAGER_H_
#define FL_INTERACTABLE_MANAGER_H_

class FLGameObject;

int fl_new_interactable(FLGameObject *owner);
void fl_delete_interactable(int handle);
void fl_interact(FLGameObject* obj);

#endif
