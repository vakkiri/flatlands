/*
 * 	tilemap.cpp
 *
 */

#include "tilemap.h"
#include "tile.h"

#include "../rendering/fl_static_rect_shader.h"
#include "../rendering/fl_textured_rect_shader.h"
#include "../rendering/rendering.h"

FLTilemap::FLTilemap(Renderer& r, unsigned int w, unsigned int h, unsigned int cell_size) : FLRenderable(r) {
	this->w = w;
	this->h = h;
	this->cell_size = cell_size;

	shader = new FLTexturedRectShader( "textured_rect_shader" );
	shader->set_projection(r.get_projection_matrix());
	shader->set_camera( glm::mat4(1.0) );
	shader->update_pc_matrix();
}

FLTilemap::~FLTilemap() {
	for (tile *t : tiles)
		delete t;
}

void FLTilemap::render() {
	((FLTexturedRectShader*) shader)->draw_rects();
}

void FLTilemap::update_shader() {
	((FLTexturedRectShader*) shader)->set_geometry( tiles );
}

void FLTilemap::add_tile(unsigned int x, unsigned int y, unsigned int w, unsigned int h, int index) {
	tile *t = new tile;
	t->bounds.x = x;
	t->bounds.y = y;
	t->bounds.w = w;
	t->bounds.h = h;
	t->u = index * cell_size;
	t->v = 0;

	tiles.push_back(t);
}

void FLTilemap::set_texture( texture *tex ) {
	((FLTexturedRectShader*) shader)->set_tex( tex );
}

void FLTilemap::set_texture( std::string name ) {
	((FLTexturedRectShader*) shader)->set_tex( name );
}

