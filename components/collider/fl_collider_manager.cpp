/*
 * 	fl_collider_manager.cpp
 *
 */

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "fl_collider_manager.h"
#include "../components.h"

const int NUM_COLLIDERS = 1000;

FLCollider colliders[NUM_COLLIDERS];
std::unordered_map<std::string, std::unordered_set<int>> collider_groups;
std::unordered_set<int> empty_set;

static int next_free = 0;
static int slots_used = 0;

int fl_new_collider( FLGameObject* owner, std::string shape ) {
	int handle = -1;
	int slots_checked = 0;

	if ( slots_used < NUM_COLLIDERS ) {

		// The slots_checked counter should not be necessary, but I really hate infinite loops
		// even if they should be impossible...
		while ( colliders[next_free].alive() && slots_checked < NUM_COLLIDERS ) {
			next_free++;
			slots_checked++;
			if ( next_free >= NUM_COLLIDERS ) {
				next_free = 0;
			}
		}
		
		handle = next_free;

		if ( colliders[handle].init( owner, shape ) ) {
			slots_used++;
		}
		else {
			handle = -1;
		}
	}

	return handle;
}

void fl_delete_collider( int handle ) {
	if ( handle >= 0 && handle < NUM_COLLIDERS && colliders[handle].alive() ) {
		colliders[handle].kill();
		for ( auto group : colliders[handle].get_collision_groups() ) {
			fl_remove_collider_from_group( handle, group );
		}
		slots_used--;
	}
}

void fl_update_colliders() {
	for ( int i = 0; i < NUM_COLLIDERS; ++i ) {
		if ( colliders[i].alive() ) {
			colliders[i].update();
		}
	}
}

void fl_add_collider_to_group( int handle, std::string group ) {
	if ( colliders[handle].alive() ) {
		colliders[handle].add_collision_group( group );

		if ( collider_groups.find( group ) == collider_groups.end() ) {
			collider_groups[group] = std::unordered_set<int>();
		}

		collider_groups[group].insert(handle);
	}
}

void fl_remove_collider_from_group( int handle, std::string group ) {
	auto grp = collider_groups.find( group );
	if ( grp != collider_groups.end() ) {
		auto hndl = grp->second.find( handle );
		if ( hndl != grp->second.end() ) {
			grp->second.erase( handle );
		}
	}
}

FLCollider* fl_get_collider( int handle ) {
	FLCollider* collider = nullptr;
	if ( handle >= 0 && handle < NUM_COLLIDERS && colliders[handle].alive() ) {
		collider = &(colliders[handle]);
	}

	return collider;
}

std::unordered_set<int>& fl_get_group_colliders( std::string group ) {
	auto it = collider_groups.find( group );
	if ( it != collider_groups.end() ) {
		return it->second;
	}
	else {
		return empty_set;
	}
}
