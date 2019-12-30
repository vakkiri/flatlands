/*
 * 	tilemap.cpp
 *
 */

#include "tilemap.h"
#include "../world/world_object.h"

#include "../logging/logging.h"
#include "../rendering/rendered_surface.h"
#include "../rendering/renderer.h"

FLTilemap::FLTilemap(Renderer& r, unsigned int w, unsigned int h, unsigned int cell_size) : FLRenderable(r) {
	this->w = w;
	this->h = h;
	this->cell_size = cell_size;

	surface = new FLTexturedSurface( r.get_textured_rect_shader() );
}

FLTilemap::~FLTilemap() {
	log_progress("Deleting tilemap");
	for (FLWorldObject *t : tiles)
		delete t;
}

void FLTilemap::render() {
	surface->render();
}

void FLTilemap::update_surface() {
	surface->update_buffers( tiles );
}

void FLTilemap::add_tile(float x, float y, float w, float h, float index) {
	FLWorldObject *t = new FLWorldObject(x, y, w, h);
	t->set_st( index * cell_size, 0 );

	tiles.push_back(t);
}

void FLTilemap::set_texture( texture *tex ) {
	// TODO: refactor class hierarchy/structure to avoid having to cast this
	((FLTexturedSurface*) surface)->set_tex( tex );
}

void FLTilemap::set_texture( std::string name ) {
	((FLTexturedSurface*) surface)->set_tex( name );
}

