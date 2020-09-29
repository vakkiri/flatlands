/*
 * 	water_renderable.h
 *
 * 	Renderable surface and buffers for water.
 *
 */

#ifndef FL_WATER_RENDERABLE_H_
#define FL_WATER_RENDERABLE_H_

#include "rendered_surface.h"
#include "fl_textured_rect_shader.h"

class FLWaterSurface : public FLTexturedSurface {
	public:
		FLWaterSurface(float x, float y, float w, float h);
		virtual void render() override;
		virtual void update_buffers();
	protected:
		float x, y, w, h;
};

#endif

