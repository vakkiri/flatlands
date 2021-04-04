/*
 * 	fl_ui_texture_element.cpp
 *
 */

#include "fl_ui_texture_element.h"
#include "rendering/fl_texture_surface.h"

FLUITextureElement::FLUITextureElement(FLTextureSurface *surface) 
	: surface(surface) {}

FLUITextureElement::~FLUITextureElement() {
}

void FLUITextureElement::render() {
	surface->push(texture);
}

