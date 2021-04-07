/*
 * 	fl_text_surface.h
 *
 *	A surface for drawing text.
 *
 */

#ifndef FL_TEXT_SURFACE_H_
#define FL_TEXT_SURFACE_H_

#include <string>
#include "rendered_surface.h"
#include "text/fl_font.h"

class FLTextShader;
class FLBitmapFont;

class FLTextSurface : public FLTexturedSurface {
	public:
		FLTextSurface();
		virtual void render();
		virtual void add_character(fl_character& c);
		virtual void add_character(float x, float y, char c);
        using FLTexturedSurface::update_buffers;
		virtual void update_buffers();

		virtual void set_font(FLBitmapFont *font);
		virtual void set_font(std::string font_name);
		virtual void set_shader(FLTextShader *shader);
		virtual void set_shader(FLTexturedRectShader *shader) = delete;
	protected:
		std::vector<fl_character> characters;
		FLBitmapFont *font;
		FLTextShader *shader;
};

#endif

