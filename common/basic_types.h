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

struct point {
	float x;
	float y;

	point( float x=0, float y=0 ) : x(x), y(y) {}

	point& operator = ( const point& other ) { 
		x = other.x;
		y = other.y;

		return *this;
	}

	point& operator += ( const point& other ) { 
		x += other.x;
		y += other.y;

		return *this;
	}

	point& operator -= ( const point& other ) { 
		x -= other.x;
		y -= other.y;

		return *this;
	}

	bool operator == ( const point& other ) const {
		return (x == other.x && y == other.y);
	}

	point operator + ( const point& other ) const {
		return point( x + other.x, y + other.y );
	}

	point operator - ( const point& other ) const {
		return point( x - other.x, y - other.y );
	}
};


struct rect {
	float x;
	float y;
	float w;
	float h;
};

struct fl_color {
	float r;
	float g;
	float b;
	float a;
};

struct fl_colored_vertex {
	point pos;
	fl_color c;
};

struct fl_textured_vertex {
	point pos;
	float s;
	float t;
};

#endif

