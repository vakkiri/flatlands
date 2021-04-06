/*
 * 	fl_texture.cpp
 *
 */

#include "rendering/renderer.h"
#include "rendering/fl_texture_surface.h"
#include "common/fl_static_buffer.h"
#include "common/fl_shape.h"
#include "components/animator/fl_animator.h"
#include "fl_texture.h"

#define DEFAULT_NUM_TEXTURES 40000

namespace FLTextures {
	FLStaticBuffer<FLTexture> textures(DEFAULT_NUM_TEXTURES);

	void init(
		fl_handle handle,
		std::string surface,
		FLShape *shape,
		float s,
		float t,
		bool reversed
	) {
		FLTexture * tex = get(handle);

		tex->surface = FLRenderer::get_texture_surface(surface);
		tex->shape = shape;
		tex->s = s;
		tex->t = t;
		tex->reversed = reversed;
		tex->visible = true;
		tex->animator = NO_ANIMATOR;
	}


	fl_handle create(
			std::string surface,
			FLShape* shape,
			float s,
			float t,
			bool reversed
	) {
		fl_handle handle = textures.create();

		if (handle != NULL_HANDLE) {
			init(handle, surface, shape, s, t, reversed);
		}

		return handle;
	}

	fl_handle create(
			std::string surface,
			FLShape *shape, 
			float s, 
			float t
	) {
		return create(surface, shape, s, t, false);
	}

	fl_handle create(
			std::string surface,
			float x,
			float y,
			float w,
			float h,
			float s,
			float t,
			bool reversed
	) {
		FLShape* shape = FLShapes::create(x, y, w, h);

		if (shape) {
			return create(surface, shape, s, t, reversed);
		} else {
			std::cout << "Warning: Could not create shape for texture.\n";
			return NULL_HANDLE;
		}
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
		return create(surface, x, y, w, h, s, t, false);
	}

	void destroy(FLTexture * tex) {
		if (!tex->external_shape) {
			FLShapes::destroy(tex->shape);
		}

		textures.destroy(tex);
	}

	void destroy(fl_handle handle) {
		FLTexture * tex = get(handle);
		destroy(tex);
	}

	FLTexture *get(fl_handle handle) {
		return &textures[handle];
	}

	void render(fl_handle handle) {
		FLTexture * tex = get(handle);
		if (tex->visible) {
			if (tex->animator != NO_ANIMATOR) {
				tex->s = FLAnimators::s(tex->animator);
				tex->t = FLAnimators::t(tex->animator);
			}

			tex->surface->push(tex);
		}
	}
}
