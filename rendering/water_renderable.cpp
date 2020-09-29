/*
 * 	water_renderable.h
 *
 */

#include <iostream>
#include "water_renderable.h"
#include "renderer.h"
#include "textured_object.h"

#include "../logging/logging.h"

FLWaterSurface::FLWaterSurface(float x, float y, float w, float h)
	: FLTexturedSurface() {
	set_shader(Renderer::getInstance().get_water_shader());
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	update_buffers();
}

void FLWaterSurface::update_buffers() {
	FLTexturedObject *shape = new FLTexturedObject(x, y, w, h);
	set_tex(Renderer::getInstance().screen_texture());
	FLTexturedSurface::update_buffers(shape);
	delete shape;
}

void FLWaterSurface::render() {
	shader->bind();
	set_tex(Renderer::getInstance().screen_texture());
	FLTexturedSurface::render();
}

