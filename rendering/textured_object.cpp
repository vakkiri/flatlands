/*
 * 	textured_object.cpp
 *
 */

#include <iostream>
#include "textured_object.h"

FLTexturedObject::FLTexturedObject( FLGameObject* owner, float x, float y, float w, float h ) {
	_x = x;
	_y = y;
	_w = w;
	_h = h;
	_s = 0;
	_t = 0;
	reverse = false;
	visible = true;
	this->owner = owner;
}

FLTexturedObject::FLTexturedObject( float x, float y, float w, float h ) : FLTexturedObject( nullptr, x, y, w, h ) {}


FLTexturedObject::FLTexturedObject( FLGameObject* owner, float w, float h ) : FLTexturedObject( owner, 0, 0, w, h ) {}

FLTexturedObject::FLTexturedObject( FLTexturedObjectParams& params ) : FLTexturedObject( params.owner, params.x, params.y, params.w, params.h ) {}

void FLTexturedObject::set_st( float s, float t ) {
	_s = s;
	_t = t;
}

void FLTexturedObject::set_x( float x ) {
	_x = x;
}

void FLTexturedObject::set_y( float y ) {
	_y = y;
}

void FLTexturedObject::set_w( float w ) {
	_w = w;
}

void FLTexturedObject::set_h( float h ) {
	_h = h;
}

float FLTexturedObject::x() { 
	float x = _x;
	if ( owner != nullptr ) {
		x += owner->x();
	}
	return x; 
}

float FLTexturedObject::y() { 
	float y = _y;
	if ( owner != nullptr ) {
		y += owner->y();
	}
	return y;
}

float FLTexturedObject::w() { return _w; }
float FLTexturedObject::h() { return _h; }
float FLTexturedObject::s() { return _s; }
float FLTexturedObject::t() { return _t; }

void FLTexturedObject::set_reverse( bool reverse ) { this->reverse = reverse; }
bool FLTexturedObject::reversed() { return reverse; }

void FLTexturedObject::set_visible( bool visible ) { this->visible = visible; }
bool FLTexturedObject::is_visible() { return visible; }
