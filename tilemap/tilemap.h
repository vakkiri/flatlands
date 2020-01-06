/*
 * 	tilemap.h
 *
 * 	Core tilemap class, including collision map and rendering integration.
 *
 */

#ifndef TILEMAP_H_
#define TILEMAP_H_

#include <vector>
#include <string>

class FLTexturedObject;
class FLTexturedSurface;
struct texture;

class FLTilemap {
	public:
		FLTilemap(unsigned int w, unsigned int h, unsigned int cell_size);
		virtual ~FLTilemap();

		void update_surface();
		void reset_collision_map();
		void set_texture( texture *tex );
		void set_texture( std::string name );
		void add_tile( float x, float y, float w, float h, float index, bool solid);
		bool solid_at( float x, float y );
	protected:
		std::vector<FLTexturedObject*> tiles;
		std::vector<std::vector<bool>> collision_map;
		unsigned int w;
		unsigned int h;
		float cell_size;
		FLTexturedSurface *surface;
};

#endif
