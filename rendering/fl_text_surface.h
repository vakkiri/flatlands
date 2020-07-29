/*
 * 	fl_text_surface.h
 *
 *	A surface for drawing text.
 *
 */

#ifndef FL_TEXT_SURFACE_H_
#define FL_TEXT_SURFACE_H_

#include "rendered_surface.h"

class FLTextShader;
class FLBitmapFont;

class FLTextSurface : public FLRenderedSurface {
	public:
		FLTextSurface();
		virtual void render();
		virtual void update_buffers();

	protected:
		FLBitmapFont *font;
		FLTextShader *shader;
};

#endif

