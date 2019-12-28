/*
 * 	tilemap.h
 *
 * 	Core tilemap class, including collision map and rendering integration.
 *
 */

#ifndef TILEMAP_H_
#define TILEMAP_H_

#include <vector>
#include "../rendering/renderable.h"

struct tile;

class FLTilemap : FLRenderable {
	public:
		FLTilemap(Renderer& r, unsigned int w, unsigned int h);
		virtual ~FLTilemap();

		void render();
		void update_shader();
	protected:
		std::vector<tile*> tiles;
		unsigned int w;
		unsigned int h;
};

#endif
