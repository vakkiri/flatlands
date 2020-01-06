/*
 * 	textured_object.cpp
 *
 */

#include "textured_object.h"

FLTexturedObject::FLTexturedObject() : FLTexturedObject( 0, 0, 0, 0 ) {}

FLTexturedObject::FLTexturedObject( float x, float y, float w, float h ) : FLWorldObject( x, y, w, h ) {
	_s = 0;
	_t = 0;
	reverse = false;
};

void FLTexturedObject::set_st( float s, float t ) {
	_s = s;
	_t = t;
}

float FLTexturedObject::s() { return _s; }
float FLTexturedObject::t() { return _t; }

void FLTexturedObject::set_reverse( bool reverse ) { this->reverse = reverse; }
bool FLTexturedObject::reversed() { return reverse; }
