/*
 * 	fl_game_object
 *
 */

#include "fl_static_buffer.h"
#include "fl_object.h"

#define DEFAULT_NUM_OBJECTS 10000

namespace FLObjects {
	FLStaticBuffer<FLObject> objects(DEFAULT_NUM_OBJECTS);

	FLObject* create() {
		return objects.create();
	}
}

FLObject::FLObject() {
}

void FLObject::destroy() {
	for (auto [name, shape] : shapes) {
		FLShapes::destroy(shape);
	}
	for (auto [name, tex] : textures) {
		FLTextures::destroy(tex);
	}
	shapes.clear();
}

void FLObject::add_shape(std::string name, float x, float y, float w, float h) {
	shapes[name] = FLShapes::create(x, y, w, h);
}

void FLObject::add_texture(std::string name, std::string image, float x, float y, float w, float h, float s, float t) {
	(void) name;
	(void) image;
	(void) x;
	(void) y;
	(void) w;
	(void) h;
	(void) s;
	(void) t;
}

FLShape* FLObject::shape(std::string name) {
	return shapes[name];
}

FLTexture* FLObject::texture(std::string name) {
	return textures[name];
}
