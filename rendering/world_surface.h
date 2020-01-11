/*
 * 	world_surface.h
 *
 * 	Constantly updating surface for dynamic world objects.
 *
 */

#ifndef WORLD_SURFACE_H_
#define WORLD_SURFACE_H_

#include <vector>
#include "rendered_surface.h"

class FLWorldSurface : public FLTexturedSurface {
	public:
		FLWorldSurface();

		virtual void update_buffers();
		virtual void add_object( FLTexturedObject* object );
		virtual void remove_object( FLTexturedObject* object );
	protected:
		std::vector< FLTexturedObject* > objects_to_render;
};


#endif

