/*
 * 	fl_texture.cpp
 *
 */

#include "../../common/fl_static_buffer.h"
#include "../../common/fl_shape.h"
#include "fl_texture.h"

#define DEFAULT_NUM_TEXTURES 10000

namespace FLTextures {
	FLStaticBuffer<FLTexture> textures(DEFAULT_NUM_TEXTURES);

	FLAccessor<FLTexture> create(FLAccessor<FLShape> shape, float s, float t, bool reverse) {
		FLAccessor<FLTexture> ret = textures.create();

		if (!ret.null()) {
			ret->init(shape, s, t, reverse);
		}

		return ret;
	}

	FLAccessor<FLTexture> create(FLAccessor<FLShape> shape, float s, float t) {
		return create(shape, s, t, false);
	}

	FLAccessor<FLTexture> create(float x, float y, float w, float h, float s, float t, bool reverse) {
		FLAccessor<FLShape> shape = FLShapes::create(x, y, w, h);

		return create(shape, s, t, reverse);
	}

	FLAccessor<FLTexture> create(float x, float y, float w, float h, float s, float t) {
		return create(x, y, w, h, s, t, false);
	}

	void destroy(FLAccessor<FLTexture> tex) {
		tex->destroy();	// call the texture destructor (free shape if necessary)
		tex.destroy();	// call the accessor destructor (free texture)
	}
}

FLTexture::FLTexture() {}

void FLTexture::init(FLAccessor<FLShape> shape, float s, float t, bool reverse) {
	this->shape = shape;
	_s = s;
	_t = t;
	_reverse = reverse;
	_visible = true;
}

void FLTexture::destroy() {
	if (!external_shape) {
		shape.destroy();
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

