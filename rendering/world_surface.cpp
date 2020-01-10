/*
 * 	world_surface.cpp
 */

#include "world_surface.h"

FLWorldSurface::FLWorldSurface() : FLTexturedSurface() {}

void FLWorldSurface::update_buffers() {
	FLTexturedSurface::update_buffers( objects_to_render );
	objects_to_render.clear();
}

void FLWorldSurface::add_object( FLTexturedObject* object ) {
	objects_to_render.push_back( object );
}

