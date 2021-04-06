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

struct FLTexture {
	FLTextureSurface *surface;
	FLShape* shape;
	bool external_shape;
	bool reversed;
	bool visible;
	float s;
	float t;
	fl_handle animator;
};

namespace FLTextures {
	fl_handle create(std::string surface, float x, float y, float w, float h, float s, float t);
	fl_handle create(std::string surface, float x, float y, float w, float h, float s, float t, bool reversed);
	fl_handle create(std::string surface, FLShape *shape, float s, float t);
	fl_handle create(std::string surface, FLShape *shape, float s, float t, bool reversed);

	FLTexture* get(fl_handle handle);

	void destroy(FLTexture* tex);
	void destroy(fl_handle handle);

	void render(fl_handle handle);
}

#endif

