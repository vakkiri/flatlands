/*
 * 	world_object.cpp
 *
 */

#include "player/player.h"
#include "world_object.h"
#include "world_environment.h"

std::vector<FLWorldObject*> world_objects;

FLWorldObject::FLWorldObject() : FLWorldObject( 0, 0, 0, 0 ) {
}

FLWorldObject::FLWorldObject( float x, float y, float w, float h ) :
	FLGameObject( x, y, w, h ) {
	wo_list_position = world_objects.size();
	world_objects.push_back( this );
}

FLWorldObject::~FLWorldObject() {
	world_objects[wo_list_position] = nullptr;
}

float FLWorldObject::bounds_x() { return x(); }
float FLWorldObject::bounds_y() { return y(); }
float FLWorldObject::bounds_w() { return w(); }
float FLWorldObject::bounds_h() { return h(); }

void clear_world_objects() {
	while ( !world_objects.empty() ) {
		// TODO: shouldn't have to check against player here
		if ( world_objects.back() != nullptr &&
		     world_objects.back() != FLWorldEnvironment::getInstance().player() )
			delete world_objects.back();
		world_objects.pop_back();
	}
	world_objects.push_back( FLWorldEnvironment::getInstance().player() );

}

