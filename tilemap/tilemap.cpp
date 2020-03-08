/*
 * 	tilemap.cpp
 *
 */

#include "tilemap.h"

#include "../logging/logging.h"
#include "../rendering/rendered_surface.h"
#include "../rendering/renderer.h"
#include "../rendering/textured_object.h"

FLTilemap::FLTilemap(unsigned int w, unsigned int h, unsigned int cell_size) {
	this->w = w;
	this->h = h;
	this->cell_size = cell_size;

	surface = Renderer::getInstance().get_tilemap_surface();

	reset_collision_map();
}

FLTilemap::~FLTilemap() {
	log_progress("Deleting tilemap");
	reset();
}

void FLTilemap::update_surface() {
	surface->update_buffers( tiles );
}

void FLTilemap::set_solid_at( float x, float y, float w, float h, bool solid ) {
	int x_cells = int(w / cell_size);
	int y_cells = int(h / cell_size);
	int x_cell;
	int y_cell;

	for (int i = 0; i < y_cells; ++i) {
		y_cell = int(y / cell_size) + i;
		for (int j = 0; j < x_cells; ++j) {
			x_cell = int(x / cell_size) + j;
			collision_map[y_cell][x_cell] = solid; 
		}
	}
}

void FLTilemap::add_tile( float x, float y, float w, float h, float index, bool solid ) {
	FLTexturedObject *t = new FLTexturedObject(x, y, w, h);
	t->set_st( index * cell_size, 16 * tileset );

	tiles.push_back(t);

	if ( solid ) {
		set_solid_at( x, y, w, h, true );
	}
}

void FLTilemap::reset_collision_map() {
	collision_map = std::vector<std::vector<bool>>( int(h / cell_size), std::vector<bool>( int(w / cell_size), false ) );
}

bool FLTilemap::solid_at( float x, float y ) {
	int _y = int(y / cell_size);
	int _x = int(x / cell_size);

	if (_x < 0 || _y < 0 || _x >= (w/cell_size) || _y >= (h/cell_size))
		return false;

	return collision_map[_y][_x];
}

void FLTilemap::reset() {
	reset_collision_map();

	while ( !tiles.empty() ) {
		delete tiles.back();
		tiles.pop_back();
	}
}

void FLTilemap::reset( unsigned int new_w, unsigned int new_h ) {
	w = new_w;
	h = new_h;

	reset();
}

void FLTilemap::set_tileset( unsigned int tileset ) {
	this->tileset = tileset;
}

