/*
 * 	tilemap.cpp
 *
 */

#include "tilemap.h"

#include "../logging/logging.h"
#include "../rendering/rendered_surface.h"
#include "../rendering/renderer.h"
#include "../rendering/textured_object.h"

FLTilemap::FLTilemap(Renderer& r, unsigned int w, unsigned int h, unsigned int cell_size) {
	this->w = w;
	this->h = h;
	this->cell_size = cell_size;

	surface = new FLTexturedSurface( r );

	reset_collision_map();
}

FLTilemap::~FLTilemap() {
	log_progress("Deleting tilemap");
	delete surface;
	for (FLTexturedObject *t : tiles)
		delete t;
}

void FLTilemap::update_surface() {
	surface->update_buffers( tiles );
}

void FLTilemap::add_tile( float x, float y, float w, float h, float index, bool solid ) {
	FLTexturedObject *t = new FLTexturedObject(x, y, w, h);
	t->set_st( index * cell_size, 0 );

	tiles.push_back(t);

	if ( solid )
		collision_map[int(y / cell_size)][int(x / cell_size)] = true;
}

void FLTilemap::set_texture( texture *tex ) {
	surface->set_tex( tex );
}

void FLTilemap::set_texture( std::string name ) {
	((FLTexturedSurface*) surface)->set_tex( name );
}

void FLTilemap::reset_collision_map() {
	collision_map = std::vector<std::vector<bool>>( int(h / cell_size), std::vector<bool>( int(w / cell_size), false ) );
}

bool FLTilemap::solid_at( float x, float y ) {
	return collision_map[int(y / cell_size)][int(x / cell_size)];
}
