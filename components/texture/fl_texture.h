/*
 * 	fl_texture.h
 *
 * 	Texturing component.
 *
 */

#ifndef FL_TEXTURE_H_
#define FL_TEXTURE_H_

#include <string>
#include "common/basic_types.h"

class FLTextureSurface;

struct FLTexture {
	FLTextureSurface *surface;
	fl_handle parent;
	bool reversed;
	bool visible;
	float x;
	float y;
	float w;
	float h;
	float s;
	float t;
};

namespace FLTextures {
	fl_handle create(std::string surface, fl_handle parent, float x, float y, float w, float h, float s, float t, bool reversed);
	fl_handle create(std::string surface, float x, float y, float w, float h, float s, float t);
	fl_handle create(std::string surface, float x, float y, float w, float h, float s, float t, bool reversed);
	fl_handle create(std::string surface, fl_handle parent, float s, float t, float w, float h);
	fl_handle create(std::string surface, fl_handle parent, float s, float t, float w, float h, bool reversed);

	FLTexture* get(fl_handle handle);

	void destroy(FLTexture* tex);
	void destroy(fl_handle handle);

	void render(fl_handle handle);

	float x(fl_handle handle);
	float y(fl_handle handle);
	float w(fl_handle handle);
	float h(fl_handle handle);
	float s(fl_handle handle);
	float t(fl_handle handle);

	bool reversed(fl_handle handle);
}

#endif

