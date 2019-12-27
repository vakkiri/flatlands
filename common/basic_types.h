/*
 *	basic_types.h
 *
 *	Basic data structures for use across the game.
 *
 */

#ifndef BASIC_TYPES_H_
#define BASIC_TYPES_H_

#include <SDL2/SDL_opengl.h>

class FLPolygonShader;

struct vec2f {
	float x;
	float y;
};

struct vec4f {
	float r;
	float g;
	float b;
	float a;
};

#endif

