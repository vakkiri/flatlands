/*
 * 	tilemap.cpp
 *
 */

#include "tilemap.h"
#include "tile.h"

#include "../rendering/fl_static_rect_shader.h"
#include "../rendering/rendering.h"

FLTilemap::FLTilemap(Renderer& r, unsigned int w, unsigned int h) : FLRenderable(r) {
	this->w = w;
	this->h = h;

	shader = new FLStaticRectShader();
	shader->set_projection(r.get_projection_matrix());
	shader->update_projection();
	shader->set_camera( glm::mat4(1.0) );
	shader->update_camera();

	tile *t = new tile;
	t->bounds.x = 0;
	t->bounds.y = 0;
	t->bounds.w = 16;
	t->bounds.h = 16;

	tiles.push_back(t);
}

FLTilemap::~FLTilemap() {
	for (tile *t : tiles)
		delete t;
}

void FLTilemap::render() {
	((FLStaticRectShader*) shader)->draw_rects();
}

void FLTilemap::update_shader() {
	std::vector<rect> tile_rects;

	for ( tile* t : tiles )
		tile_rects.push_back(t->bounds);

	((FLStaticRectShader*) shader)->set_geometry( tile_rects );
}


