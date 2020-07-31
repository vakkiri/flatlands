/*
 * 	world_surface.cpp
 */

#include "world_surface.h"
#include "../logging/logging.h"
#include <algorithm>

FLWorldSurface::FLWorldSurface() : FLTexturedSurface() {}

void FLWorldSurface::update_buffers() {
	FLTexturedSurface::update_buffers(objects_to_render);
}

void FLWorldSurface::add_object(FLTexturedObject *object) {
	objects_to_render.push_back(object);
}

void FLWorldSurface::remove_object(FLTexturedObject *object) {
	auto position =
		std::find(objects_to_render.begin(), objects_to_render.end(), object);

	if (position != objects_to_render.end()) {
		objects_to_render.erase(position);
	}
}

void FLWorldSurface::clear() {
	for (auto obj : objects_to_render) {
		if (obj != nullptr)
			log_warning("Non-null object during world surface clear");
	}
	objects_to_render.clear();
}
