/*
 * 	world_surface.h
 *
 * 	Constantly updating surface for dynamic world objects.
 *
 */

#ifndef WORLD_SURFACE_H_
#define WORLD_SURFACE_H_

#include "rendered_surface.h"
#include <vector>

class FLWorldSurface : public FLTexturedSurface {
  public:
	FLWorldSurface();

    using FLTexturedSurface::update_buffers;
	virtual void update_buffers();
	virtual void add_object(FLTexturedObject *object);
	virtual void remove_object(FLTexturedObject *object);
	virtual void clear();

  protected:
	std::vector<FLTexturedObject *> objects_to_render;
};

#endif
