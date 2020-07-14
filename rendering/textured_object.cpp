/*
 * 	textured_object.cpp
 *
 */

#include "textured_object.h"
#include <iostream>

FLTexturedObject::FLTexturedObject(FLShape *parent, float x, float y,
								   float w, float h) {
	_x = x;
	_y = y;
	_w = w;
	_h = h;
	_s = 0;
	_t = 0;
	reverse = false;
	visible = true;
	this->parent = parent;
}

FLTexturedObject::FLTexturedObject(float x, float y, float w, float h)
	: FLTexturedObject(nullptr, x, y, w, h) {}

FLTexturedObject::FLTexturedObject(FLShape *parent, float w, float h)
	: FLTexturedObject(parent, 0, 0, w, h) {}

FLTexturedObject::FLTexturedObject(FLTexturedObjectParams &params)
	: FLTexturedObject(params.parent, params.x, params.y, params.w, params.h) {}

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
