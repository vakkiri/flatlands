/*
 * 	fl_physics_handler_manager.h
 *
 * 	Management of physics handler components.
 */

#ifndef FL_PHYSICS_HANDLER_MANAGER_H_
#define FL_PHYSICS_HANDLER_MANAGER_H_

#include <string>

class FLGameObject;
class FLPhysicsHandler;

int new_physics_handler(FLGameObject *owner, std::string collider_name);
void delete_physics_handler(int handle);
void update_physics_handlers();
FLPhysicsHandler *get_physics_handler(int handle);

#endif
