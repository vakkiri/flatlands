/*
 * 	fl_shape.h
 *
 * 	Shape class.
 *
 */

#ifndef FL_SHAPE_H_
#define FL_SHAPE_H_

#include <vector>
#include "basic_types.h"

class FLShape {
	public:
		FLShape( float x, float y, float w, float h );
		FLShape( std::vector<point> vertices );
		
		std::vector<point>& get_vertices();
		void translate( float x, float y );
		void translate( const point& amt );
		void set_pos( float x, float y );
		void set_pos( const point& p );
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

#endif

