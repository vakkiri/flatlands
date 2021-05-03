/*
 * 	fl_texture.cpp
 *
 */

#include "rendering/renderer.h"
#include "rendering/fl_texture_surface.h"
#include "common/fl_object.h"
#include "common/fl_static_buffer.h"
#include "fl_texture.h"

#define DEFAULT_NUM_TEXTURES 40000

namespace FLTextures {
	FLStaticBuffer<FLTexture> textures(DEFAULT_NUM_TEXTURES);

	void init(
		fl_handle handle,
		std::string surface,
		fl_handle parent,
		float x,
		float y,
		float w,
		float h,
		float s,
		float t,
		bool reversed
	) {
		FLTexture * tex = get(handle);

		tex->surface = FLRenderer::get_texture_surface(surface);
		tex->parent = parent;
		tex->x = x;
		tex->y = y;
		tex->w = w;
		tex->h = h;
		tex->s = s;
		tex->t = t;
		tex->reversed = reversed;
		tex->visible = true;
	}

	fl_handle create(
			std::string surface,
			fl_handle parent,
			float x,
			float y,
			float w,
			float h,
			float s,
			float t,
			bool reversed
	) {
		fl_handle handle = textures.create();

		if (handle != NULL_HANDLE) {
			init(handle, surface, parent, x, y, w, h, s, t, reversed);
		} else {
			std::cout << "Warning: could not create texture.\n";
		}

		return handle;
	}

	fl_handle create(
			std::string surface,
			fl_handle parent,
			float s,
			float t,
			float w,
			float h,
			bool reversed
	) {
		// If we don't supply x/y, assume we want to use
		// the parent position.
		return create(surface, parent, 0, 0, w, h, s, t, reversed);
	}

	fl_handle create(
			std::string surface,
			fl_handle parent, 
			float s, 
			float t,
			float w,
			float h
	) {
		return create(surface, parent, s, t, w, h, false);
	}

	fl_handle create(
			std::string surface,
			float x,
			float y,
			float w,
			float h,
			float s,
			float t
	) {
		return create(surface, NULL_HANDLE, x, y, w, h, s, t, false);
	}

	void destroy(FLTexture * tex) {
		// refresh is done to make sure dead elements don't linger
		tex->surface->refresh();
		textures.destroy(tex);
	}

	void destroy(fl_handle handle) {
		FLTexture * tex = FLTextures::get(handle);
		tex->surface->refresh();
		textures.destroy(handle);
	}

	FLTexture *get(fl_handle handle) {
		if (handle != NULL_HANDLE) {
			return &textures[handle];
		} else {
			std::cout << "Warning: tried to access texture with null handle.\n";
			return nullptr;
		}
	}

	void render(fl_handle handle) {
		FLTexture * tex = get(handle);
		if (tex->visible) {
			tex->surface->push(handle);
		} else {
			// this is necessary to make sure an object turning
			// invisible still updates the screen
			tex->surface->refresh();
		}
	}

	float x(fl_handle handle) {
		float x = textures[handle].x;

		if (textures[handle].parent != NULL_HANDLE) {
			x += FLObjects::x(textures[handle].parent);
		}

		return x;
	}

	float y(fl_handle handle) {
		float y = textures[handle].y;

		if (textures[handle].parent != NULL_HANDLE) {
			y += FLObjects::y(textures[handle].parent);
		}

		return y;
	}

	float w(fl_handle handle) {
		return textures[handle].w;
	}

	float h(fl_handle handle) {
		return textures[handle].h;
	}

	float s(fl_handle handle) {
		return textures[handle].s;
	}

	float t(fl_handle handle) {
		return textures[handle].t;
	}

	bool reversed(fl_handle handle) {
		return textures[handle].reversed;
	}

	void set_pos(fl_handle handle, float x, float y) {
		textures[handle].x = x;
		textures[handle].y = y;
	}

	void set_visible(fl_handle handle, bool visible) {
		textures[handle].visible = visible;
	}
}
