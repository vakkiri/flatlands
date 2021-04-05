/*
 * 	fl_shape.cpp
 *
 * 	Shape class implementation.
 *
 */

#include <iostream>

#include "fl_static_buffer.h"
#include "fl_shape.h"

#define DEFAULT_NUM_SHAPES 40000

namespace FLShapes {
	FLStaticBuffer<FLShape> shapes(DEFAULT_NUM_SHAPES);

	FLShape* create(FLShape* parent, float x, float y, float w, float h) {
		FLShape* ret = shapes.create();

		if (ret) {
			ret->init(x, y, w, h);
			ret->set_parent(parent);
		}

		return ret;
	}

	FLShape* create(float x, float y, float w, float h) {
		return create(nullptr, x, y, w, h);
	}

	void destroy(FLShape* shape) {
		shapes.destroy(shape);
	}
}

// Default to a 0x0 rect
FLShape::FLShape() : FLShape(0, 0, 0, 0) {}

// Rect constructor
FLShape::FLShape(float x, float y, float w, float h) {
	parent = nullptr;

	_x = x;
	_y = y;
	_w = w;
	_h = h;

	// top left
	vertices.push_back(point(x, y));
	// top right
	vertices.push_back(point(x + w, y));
	// bottom right
	vertices.push_back(point(x + w, y + h));
	// bottom left
	vertices.push_back(point(x, y + h));
}

FLShape::FLShape(std::vector<point> vertices) {
	if (vertices.size() < 3) {
		std::cout << "Invalid shape: not enough vertices.\n";
	}

	// Current implementation uses a bounding box as the "size" for width and
	// height. One possible alternative would be to use a radius for a circular
	// bounding shape.
	else {
		float min_x = vertices[0].x;
		float min_y = vertices[0].y;
		float max_x = vertices[0].x;
		float max_y = vertices[0].y;

		for (point p : vertices) {
			if (p.x < min_x)
				min_x = p.x;
			if (p.y < min_y)
				min_y = p.y;
			if (p.x > max_x)
				max_x = p.x;
			if (p.y > max_y)
				max_y = p.y;
		}

		_x = min_x;
		_y = min_y;
		_w = max_x - min_x;
		_h = max_y - min_y;
	}
}

// This init function assumes a rect
void FLShape::init(float x, float y, float w, float h) {
	set_pos(x, y);
	_w = w;
	_h = h;
}

float FLShape::x() { 
	float out = _x;
	if (parent != nullptr) {
		out += parent->x();
	}
	return out; 
}

float FLShape::y() { 
	float out = _y;
	if (parent != nullptr) {
		out += parent->y();
	}
	return out; 
}

float FLShape::w() { return _w; }
float FLShape::h() { return _h; }

std::vector<point> &FLShape::get_vertices() { return vertices; }

void FLShape::translate(float x, float y) { translate(point(x, y)); }

void FLShape::translate(const point &amt) {
	for (point p : vertices) {
		p += amt;
	}
	_x += amt.x;
	_y += amt.y;
}

void FLShape::set_parent(FLShape *parent) { this->parent = parent; }

void FLShape::set_pos(float x, float y) { set_pos(point(x, y)); }

void FLShape::set_pos(const point &p) {
	float dx = p.x - _x;
	float dy = p.y - _y;

	translate(point(dx, dy));
}
