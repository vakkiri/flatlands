/*
 * 	world_surface.cpp
 */

#include <algorithm>
#include "world_surface.h"

FLWorldSurface::FLWorldSurface() : FLTexturedSurface() {}

void FLWorldSurface::update_buffers() {
	FLTexturedSurface::update_buffers( objects_to_render );
}

void FLWorldSurface::add_object( FLTexturedObject* object ) {
	objects_to_render.push_back( object );
}

void FLWorldSurface::remove_object( FLTexturedObject* object ) {
	auto position = std::find( objects_to_render.begin(), objects_to_render.end(), object );

	if ( position != objects_to_render.end() )
		objects_to_render.erase( position );
}

