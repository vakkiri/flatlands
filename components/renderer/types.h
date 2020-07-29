/*
 * 	types.h
 *
 * 	types relevant to the 
 *
 */

#ifndef FL_RENDERER_TYPES_H_
#define FL_RENDERER_TYPES_H_

struct texture;

struct fl_frame {
	texture *tex;
	float s;
	float t;
	float w;
	float h;
};

#endif

