/*
 *	fl_static_rect_shader.h
 *
 * 	A shader meant to efficiently draw batches of uniformly sized rects.
 */

#ifndef FL_STATIC_RECT_SHADER_H_
#define FL_STATIC_RECT_SHADER_H_

#include <vector>
#include "../common/basic_types.h"
#include "fl_polygon_shader.h"

class FLStaticRectShader : public FLPolygonShader {
	public:
		FLStaticRectShader();
		virtual bool create_program( std::string program_name );
		virtual void draw_rects();

		void set_geometry(std::vector<rect> &rects);
	protected:
		GLuint rect_vbo;
		GLuint rect_ibo;
		GLuint rect_vao;
		unsigned int num_indices;
		unsigned int num_verts;
};

#endif
