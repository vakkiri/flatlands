/*
 * 	fl_texture.h
 *
 * 	Texturing component.
 *
 */

#ifndef FL_TEXTURE_H_
#define FL_TEXTURE_H_

#include "../../common/fl_accessor.h"
#include "../../common/fl_shape.h"

class FLTexture {
	public:
		FLTexture();

		void init(FLAccessor<FLShape> shape, float s, float t, bool reverse);
		void destroy();

		void set_reverse(bool reverse);
		bool reversed();

		void set_visible(bool visible);
		bool visible();

		float x();
		float y();
		float w();
		float h();
		float s();
		float t();

	private:
		FLAccessor<FLShape> shape;
		bool external_shape;
		bool _reverse;
		bool _visible;
		float _s;
		float _t;
};

namespace FLTextures {
	FLAccessor<FLTexture> create(float x, float y, float w, float h, float s, float t);
	FLAccessor<FLTexture> create(float x, float y, float w, float h, float s, float t, bool reverse);
	FLAccessor<FLTexture> create(FLShape *shape, float s, float t);
	FLAccessor<FLTexture> create(FLShape *shape, float s, float t, bool reverse);

	void destroy(FLAccessor<FLTexture> tex);
}

#endif

