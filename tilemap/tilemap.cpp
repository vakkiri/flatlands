/*
 * 	tilemap.cpp
 *
 */

#include "tilemap.h"
#include "tile.h"

#include "../logging/logging.h"
#include "../rendering/rendered_surface.h"
#include "../rendering/renderer.h"

FLTilemap::FLTilemap(Renderer& r, unsigned int w, unsigned int h, unsigned int cell_size) {
	this->w = w;
	this->h = h;
	this->cell_size = cell_size;

	surface = new FLTexturedSurface( r );
}

FLTilemap::~FLTilemap() {
	log_progress("Deleting tilemap");
	delete surface;
	for (FLTexturedObject *t : tiles)
		delete (FLTile*) t;
}

void FLTilemap::update_surface() {
	surface->update_buffers( tiles );
}

void FLTilemap::add_tile(float x, float y, float w, float h, float index) {
	FLTile *t = new FLTile(x, y, w, h);
	t->set_st( index * cell_size, 0 );

	tiles.push_back(t);
}

void FLTilemap::set_texture( texture *tex ) {
	surface->set_tex( tex );
}

void FLTilemap::set_texture( std::string name ) {
	((FLTexturedSurface*) surface)->set_tex( name );
}

