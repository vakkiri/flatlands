/*
 *	fl_static_rect_shader.h
 *
 * 	A shader meant to efficiently draw batches of uniformly sized rects.
 */

#ifndef FL_STATIC_RECT_SHADER_H_
#define FL_STATIC_RECT_SHADER_H_

#include "fl_polygon_shader.h"

class FLStaticRectShader : public FLPolygonShader {
	public:
		FLStaticRectShader() : FLPolygonShader(){};
		virtual bool create_program( std::string program_name );
		virtual void draw_rects8();
		virtual void draw_rects16();
		virtual void draw_rects32();

	protected:
		virtual bool init_poly_buffers();

	private:
		GLuint rect8_vao;
		GLuint rect16_vao;
		GLuint rect32_vao;

};

#endif
