/*
 * 	tilemap.h
 *
 * 	Core tilemap class, including collision map and rendering integration.
 *
 */

#ifndef TILEMAP_H_
#define TILEMAP_H_

#include "../common/common.h"
#include "../components/texture/fl_texture.h"

#include <vector>
#include <string>

class FLTextureSurface;
struct texture;

class FLTilemap {
  public:
	FLTilemap(unsigned int w, unsigned int h, unsigned int cell_size);
	virtual ~FLTilemap();

	void load_map(int id);
	void update_surface();
	void reset();
	void reset(unsigned int new_w, unsigned int new_h);
	void reset_collision_map();
	void add_tile(float x, float y, float w, float h, float index, bool solid,
				  int layer);
	bool solid_at(float x, float y);
	void set_solid_at(float x, float y);
	void set_tileset(unsigned int tileset);

	bool touches_shape(FLShape *shape);
	bool touches_line(point p1, point p2);

	float get_cell_size();

  protected:
	std::vector<std::vector<bool>> collision_map;
	unsigned int w;
	unsigned int h;
	unsigned int tileset;
	float cell_size;
	FLTextureSurface *bgsurface;
	FLTextureSurface *fgsurface;
	std::vector<FLTexture*> tiles;
};

#endif
