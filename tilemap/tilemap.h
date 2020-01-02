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
class Renderer;
struct texture;

class FLTilemap {
	public:
		FLTilemap(Renderer& r, unsigned int w, unsigned int h, unsigned int cell_size);
		virtual ~FLTilemap();

		void update_surface();
		void set_texture( texture *tex );
		void set_texture( std::string name );
		void add_tile( float x, float y, float w, float h, float index );
	protected:
		std::vector<FLTexturedObject*> tiles;
		unsigned int w;
		unsigned int h;
		float cell_size;
		FLTexturedSurface *surface;
};

#endif
