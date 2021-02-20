/*
 * 	fl_shape.h
 *
 * 	Shape class.
 *
 */

#ifndef FL_SHAPE_H_
#define FL_SHAPE_H_

#include "basic_types.h"
#include "fl_accessor.h"
#include <vector>


class FLShape {
	public:
		FLShape();
		FLShape(float x, float y, float w, float h);
		FLShape(std::vector<point> vertices);

		void init(float x, float y, float w, float h);

		std::vector<point> &get_vertices();
		void translate(float x, float y);
		void translate(const point &amt);
		void set_pos(float x, float y);
		void set_pos(const point &p);
		float x();
		float y();
		float w();
		float h();

	protected:
		std::vector<point> vertices;
		float _x;
		float _y;
		float _w;
		float _h;
};

namespace FLShapes {
	FLAccessor<FLShape> create(float x, float y, float w, float h);
}

#endif
