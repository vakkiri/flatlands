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
#include "../rendering/renderable.h"

struct tile;
struct texture;

class FLTilemap : FLRenderable {
	public:
		FLTilemap(Renderer& r, unsigned int w, unsigned int h, unsigned int cell_size);
		virtual ~FLTilemap();

		void render();
		void update_shader();
		void set_texture( texture *tex );
		void set_texture( std::string name );
		void add_tile( unsigned int x, unsigned int y, unsigned int w, unsigned int h, int index );
	protected:
		std::vector<tile*> tiles;
		unsigned int w;
		unsigned int h;
		unsigned int cell_size;
};

#endif
