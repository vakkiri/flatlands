/*
 * 	renderable.cpp
 *
 */

#include "rendered_surface.h"
#include "renderable.h"
#include "renderer.h"

FLRenderable::FLRenderable(Renderer &r) {
	r.add_renderable(this);
	visible = true;
	surface = nullptr;
}

FLRenderable::~FLRenderable() {
	if ( surface != nullptr )
		delete surface;
}


