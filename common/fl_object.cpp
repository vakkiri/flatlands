/*
 * 	fl_game_object
 *
 */

#include "fl_object.h"

FLObject::FLObject() {
}

void FLObject::destroy() {
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

FLAccessor<FLShape> FLObject::shape(std::string name) {
	return shapes[name];
}

/*
FLAccessor<FLTexture> FLObject::texture(std::string name) {
	return std::get<FLAccessor<FLTexture>>(components[name]);
}
*/

