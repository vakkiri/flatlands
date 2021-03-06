/*
 * 	tilemap.cpp
 *
 */

#include "tilemap.h"
#include <iostream>
#include <math.h>

#include "../logging/logging.h"
#include "../rendering/fl_texture_surface.h"
#include "../rendering/renderer.h"
#include "../rendering/textured_object.h"
#include "../resources/fl_resources.h"

FLTilemap::FLTilemap(unsigned int w, unsigned int h, unsigned int cell_size) {
	tiles.reserve(20000);

	tileset = 0;
	this->w = w;
	this->h = h;
	this->cell_size = cell_size;

	bgsurface = FLRenderer::get_texture_surface("bg_tiles");
	fgsurface = FLRenderer::get_texture_surface("fg_tiles");

	reset_collision_map();
}

FLTilemap::~FLTilemap() {
	log_progress("Deleting tilemap");
	reset();
}

void FLTilemap::set_solid_at(float x, float y) { collision_map[y][x] = true; }

void FLTilemap::add_tile(float x, float y, float w, float h, float index,
						 bool solid, int layer) {
	// TODO: cell size should be loaded from a config file
	FLResources &res = FLResources::getInstance();
	float s = index * cell_size;
	float t = 16 * tileset;
	std::string surface = layer == 0 ? "bg_tiles" : "fg_tiles";

	fl_handle tex = FLTextures::create(
		surface, x, y, w, h, s, t
	);

	if (tex != NULL_HANDLE) {
		tiles.push_back(tex);
		FLTextures::render(tex);
	} else {
		std::cout << "Warning: could not create tile texture\n";
	}

	if (tex != NULL_HANDLE) {
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
		std::cout << "warning: position out of map bounds (" << x << ","<<y<<")" << std::endl;
		return false;
	}

	return collision_map[y][x];
}

void FLTilemap::reset() {
	reset_collision_map();

	for (auto tile : tiles) {
		FLTextures::destroy(tile);
	}

	tiles.clear();
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
