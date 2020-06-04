/*
 * 	fl_update_manager.h
 *
 * 	Service for objects with per-frame updates.
 */

#ifndef FL_UPDATE_MANAGER_H_
#define FL_UPDATE_MANAGER_H_

class FLGameObject;

void update_updators();
int new_updator(FLGameObject *obj);
void delete_updator(int handle);

#endif
