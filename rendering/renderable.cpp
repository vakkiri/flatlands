/*
 * 	renderable.cpp
 *
 */

#include "rendering.h"
#include "renderable.h"

FLRenderable::FLRenderable(Renderer &r) {
	r.add_renderable(this);
	visible = true;
	shader = nullptr;
}


