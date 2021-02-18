/*
 * 	textured_object.cpp
 *
 */

#include "textured_object.h"
#include <iostream>

#define NUM_TEXTURERS 10000

static std::vector<FLTexturedObject> texturers(NUM_TEXTURERS);

int new_texturer() {
	// TODO: optimize by keeping track of last used position, instead of starting from 0
	int handle = -1;

	for (unsigned int i = 0; i < texturers.size(); ++i) {
		if (!texturers[i].alive()) {
			texturers[i].set_alive();
			handle = i;
			break;
		}
	}

	return handle;
}

int new_texturer(float x, float y, float w, float h, float s, float t) {
	int handle = new_texturer();
	if (handle >= 0) {
		FLTexturedObject* obj = get_texturer(handle);
		obj->set_x(x);
		obj->set_y(y);
		obj->set_w(w);
		obj->set_h(h);
		obj->set_st(s, t);
	}
	return handle;
}

FLTexturedObject* get_texturer(int handle) {
	if (handle < 0) {
		std::cout << "WARNING: get_texturer called with negative handle.\n";
		return nullptr;
	}
	if (texturers[handle].alive()) {
		return &texturers[handle];
	} else {
		return nullptr;
	}
}

void delete_texturer(int handle) {
	if (handle > 0) {
		texturers[handle].kill();
	} else {
		std::cout << "WARNING: get_texturer called with negative handle.\n";
	}
}

FLTexturedObject::FLTexturedObject() {
	parent = nullptr;
	_alive = false;
	reverse = false;
	visible = true;
	_s = 0;
	_t = 0;
}

FLTexturedObject::FLTexturedObject(FLTexturedObjectParams &params) 
	: FLTexturedObject() {
	parent = params.parent;
	_x = params.x;
	_y = params.y;
	_w = params.w;
	_h = params.h;
}

FLTexturedObject::FLTexturedObject(float x, float y, float w, float h) 
	: FLTexturedObject() {
	_x = x;
	_y = y;
	_w = w;
	_h = h;
}

bool FLTexturedObject::alive() {
	return _alive;
}

void FLTexturedObject::kill() {
	parent = nullptr;
	_alive = false;
}

void FLTexturedObject::set_alive() {
	_alive = true;
}

void FLTexturedObject::init(float x, float y, float w, float h, float s, float t) {
	_x = x;
	_y = y;
	_w = w;
	_h = h;
	_s = s;
	_t = t;
	visible = true;
	reverse = false;
	parent = nullptr;
}

void FLTexturedObject::set_st(float s, float t) {
	_s = s;
	_t = t;
}

void FLTexturedObject::set_x(float x) { _x = x; }

void FLTexturedObject::set_y(float y) { _y = y; }

void FLTexturedObject::set_w(float w) { _w = w; }

void FLTexturedObject::set_h(float h) { _h = h; }

float FLTexturedObject::x() {
	float x = _x;
	if (parent != nullptr) {
		x += parent->x();
	}
	return x;
}

float FLTexturedObject::y() {
	float y = _y;
	if (parent != nullptr) {
		y += parent->y();
	}
	return y;
}

float FLTexturedObject::w() { return _w; }
float FLTexturedObject::h() { return _h; }
float FLTexturedObject::s() { return _s; }
float FLTexturedObject::t() { return _t; }

void FLTexturedObject::set_reverse(bool reverse) { this->reverse = reverse; }
bool FLTexturedObject::reversed() { return reverse; }

void FLTexturedObject::set_visible(bool visible) { this->visible = visible; }
bool FLTexturedObject::is_visible() { return visible; }
