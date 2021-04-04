/*
 * 	fl_texture.cpp
 *
 */

#include "rendering/renderer.h"
#include "rendering/fl_texture_surface.h"
#include "../../common/fl_static_buffer.h"
#include "../../common/fl_shape.h"
#include "fl_texture.h"

#define DEFAULT_NUM_TEXTURES 10000

namespace FLTextures {
	FLStaticBuffer<FLTexture> textures(DEFAULT_NUM_TEXTURES);

	FLTexture* create(
			std::string surface,
			FLShape* shape,
			float s,
			float t,
			bool reverse
	) {
		FLTexture* ret = textures.create();

		if (ret) {
			ret->init(surface, shape, s, t, reverse);
		}

		return ret;
	}

	FLTexture* create(
			std::string surface,
			FLShape *shape, 
			float s, 
			float t
	) {
		return create(surface, shape, s, t, false);
	}

	FLTexture* create(
			std::string surface,
			float x,
			float y,
			float w,
			float h,
			float s,
			float t,
			bool reverse
	) {
		FLShape* shape = FLShapes::create(x, y, w, h);

		return create(surface, shape, s, t, reverse);
	}

	FLTexture* create(
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
		tex->destroy();	// call the texture destructor (free shape if necessary)
		textures.destroy(tex);
	}
}

FLTexture::FLTexture() {}

void FLTexture::init(std::string surface, FLShape* shape, float s, float t, bool reverse) {
	this->surface = FLRenderer::get_texture_surface(surface);
	this->shape = shape;
	_s = s;
	_t = t;
	_reverse = reverse;
	_visible = true;
}

void FLTexture::destroy() {
	if (!external_shape) {
		FLShapes::destroy(shape);
	}
}

void FLTexture::render() {
	if (_visible) {
		surface->push(this);
	}
}

void FLTexture::set_reverse(bool reverse) { _reverse = reverse; }

void FLTexture::set_visible(bool visible) { _visible = visible; }

bool FLTexture::reversed() { return _reverse; }

bool FLTexture::visible() { return _visible; }

float FLTexture::x() { return shape->x(); }

float FLTexture::y() { return shape->y(); }

float FLTexture::w() { return shape->w(); }

float FLTexture::h() { return shape->h(); }

float FLTexture::s() { return _s; }

float FLTexture::t() { return _t; }

