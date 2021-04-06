/*
 * 	fl_texture.h
 *
 * 	Texturing component.
 *
 */

#ifndef FL_TEXTURE_H_
#define FL_TEXTURE_H_

#include <string>
#include "common/fl_shape.h"
#include "common/basic_types.h"

class FLTextureSurface;
class FLAnimator;

class FLTexture {
	public:
		FLTexture();

		void init(std::string surface, FLShape* shape, float s, float t, bool reverse);
		void destroy();

		void render();

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
		FLTextureSurface *surface;
		FLShape* shape;
		bool external_shape;
		bool _reverse;
		bool _visible;
		float _s;
		float _t;
		fl_handle animator;
};

namespace FLTextures {
	FLTexture* create(std::string surface, float x, float y, float w, float h, float s, float t);
	FLTexture* create(std::string surface, float x, float y, float w, float h, float s, float t, bool reverse);
	FLTexture* create(std::string surface, FLShape *shape, float s, float t);
	FLTexture* create(std::string surface, FLShape *shape, float s, float t, bool reverse);

	void destroy(FLTexture* tex);
}

#endif

