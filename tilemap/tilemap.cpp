/*
 * 	tilemap.cpp
 *
 */

#include "tilemap.h"
#include <iostream>
#include <math.h>

#include "../logging/logging.h"
#include "../rendering/rendered_surface.h"
#include "../rendering/renderer.h"
#include "../rendering/textured_object.h"
#include "../resources/fl_resources.h"

FLTilemap::FLTilemap(unsigned int w, unsigned int h, unsigned int cell_size) 
	: bg_tiles(10000), fg_tiles(10000) {
	tileset = 0;
	this->w = w;
	this->h = h;
	this->cell_size = cell_size;

	bgsurface = Renderer::getInstance().get_tilemap_bg_surface();
	fgsurface = Renderer::getInstance().get_tilemap_fg_surface();

	reset_collision_map();
}

FLTilemap::~FLTilemap() {
	log_progress("Deleting tilemap");
	reset();
}

void FLTilemap::update_surface() {
	bgsurface->update_buffers(bg_tiles.buf());
	fgsurface->update_buffers(fg_tiles.buf());
}

void FLTilemap::set_solid_at(float x, float y) { collision_map[y][x] = true; }

void FLTilemap::add_tile(float x, float y, float w, float h, float index,
						 bool solid, int layer) {
	// TODO: cell size should be loaded from a config file
	FLResources &res = FLResources::getInstance();
	float s = index * cell_size;
	float t = 16 * tileset;
	int id = -1;

	if (layer == 0) {
		id = bg_tiles.create();

		if (id >= 0) {
			bg_tiles[id].init(x, y, w, h, s, t);
		}
	} else if (layer == 1) {
		id = fg_tiles.create();

		if (id >= 0) {
			fg_tiles[id].init(x, y, w, h, s, t);
		}
	} else {
		std::cout << "ERROR: Invalid layer\n";
	}

	if (id >= 0) {
		if (solid) {
			for (int i = 0; i < 16; ++i) {
				for (int j = 0; j < 16; ++j) {
					if (res.get_image_transparency("tiles", s + i, t + j) > 0.0) {
						set_solid_at(x + i, y + j);
					}
				}
			}
		}
	}
}

void FLTilemap::reset_collision_map() {
	collision_map = std::vector<std::vector<bool>>(
		int(h), std::vector<bool>(int(w), false));
}

bool FLTilemap::solid_at(float x, float y) {
	if (x < 0 || y < 0 || x >= w || y >= h) {
		std::cout << "warning: position out of map bounds" << std::endl;
		return false;
	}

	return collision_map[y][x];
}

void FLTilemap::reset() {
	reset_collision_map();

	// TODO: this should not be necessary but requires refactoring
	for (unsigned int i = 0; i < bg_tiles.size(); ++i) {
		bg_tiles[i].set_visible(false);
	}
	for (unsigned int i = 0; i < fg_tiles.size(); ++i) {
		fg_tiles[i].set_visible(false);
	}

	bg_tiles.clear();
	fg_tiles.clear();
}

void FLTilemap::reset(unsigned int new_w, unsigned int new_h) {
	w = new_w;
	h = new_h;

	reset();
}

void FLTilemap::set_tileset(unsigned int tileset) { this->tileset = tileset; }

bool FLTilemap::touches_shape(FLShape *shape) {
	float startx = shape->x();
	float endx = startx + shape->w();
	float starty = shape->y();
	float endy = starty + shape->h();

	for (float x = startx; x < endx; ++x) {
		for (float y = starty; y < endy; ++y) {
			if (solid_at(x, y)) {
				return true;
			}
		}
	}

	return false;
}

bool FLTilemap::touches_line(point p1, point p2) {
	// TODO: base this on distance from p1 to p2, for large objects
	// num steps should be distance / cell size but im so lazy rn
	float num_steps = 4;
	float xstep = (p2.x - p1.x) / num_steps;
	float ystep = (p2.y - p1.y) / num_steps;
	float x = p1.x;
	float y = p1.y;

	for (int i = 0; i < num_steps; ++i) {
		if (solid_at(x, y)) {
			return true;
		}
		x += xstep;
		y += ystep;
	}

	return false;
}

float FLTilemap::get_cell_size() { return cell_size; }
