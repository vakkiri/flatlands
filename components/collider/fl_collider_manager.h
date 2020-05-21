/*
 * 	fl_collider_manager.h
 *
 * 	Management of collision components.
 */

#ifndef FL_COLLIDER_HANDLER_MANAGER_H_
#define FL_COLLIDER_HANDLER_MANAGER_H_

#include <string>

class FLGameObject;
class FLCollider;

int fl_new_collider( FLGameObject* owner, std::string shape, std::string name );
void fl_delete_collider( int handle );
void fl_update_colliders();
void fl_add_collider_to_group( int handle, std::string group );
void fl_remove_collider_from_group( int handle, std::string group );

FLCollider* fl_get_collider( int handle );

#endif

