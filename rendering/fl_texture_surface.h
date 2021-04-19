/*
 * 	fl_texture_surface.h
 *
 * 	Surface for drawing textured quads.
 *
 */

#ifndef FL_TEXTURE_SURFACE_H_
#define FL_TEXTURE_SURFACE_H_

#include <string>

#include "common/basic_types.h"
#include "rendering/rendered_surface.h"
#include "rendering/texture.h"

class FLCamera;

class FLTextureSurface : public FLRenderedSurface {
	public:
		FLTextureSurface();
		FLTextureSurface(texture tex);
		FLTextureSurface(bool persistent, texture tex);
		FLTextureSurface(bool persistent, std::string tex);

		void push(fl_handle element);
		void refresh();
		void render();
		void set_tex(texture tex);
		void set_tex(std::string name);
		void set_camera(FLCamera *camera);

	private:
		FLCamera *camera;
		void update_buffers();
		bool persistent;
		bool dirty;
		int rendered_indices;
		texture tex;
		std::vector<fl_handle> elements;
};

#endif

