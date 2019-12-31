/*
 * 	textured_object.cpp
 *
 */

#include "textured_object.h"

FLTexturedObject::FLTexturedObject() {
	_s = 0;
	_t = 0;
}

void FLTexturedObject::set_st( float s, float t ) {
	_s = s;
	_t = t;
}

float FLTexturedObject::s() { return _s; }
float FLTexturedObject::t() { return _t; }

