/*
 * 	world_object.cpp
 *
 */

#include "world_object.h"
#include "world_environment.h"

FLWorldObject::FLWorldObject() : FLGameObject() {
	FLWorldEnvironment::getInstance().add_object( this );
}

FLWorldObject::FLWorldObject( float x, float y, float w, float h ) :
	FLGameObject( x, y, w, h ) {

	FLWorldEnvironment::getInstance().add_object( this );
}

FLWorldObject::~FLWorldObject() {
	FLWorldEnvironment::getInstance().remove_object( this );
}

float FLWorldObject::bounds_x() { return x(); }
float FLWorldObject::bounds_y() { return y(); }
float FLWorldObject::bounds_w() { return w(); }
float FLWorldObject::bounds_h() { return h(); }

