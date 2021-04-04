/*
 * 	fl_ui_texture_element.h
 *
 * 	UI Elements drawn with a texture.
 *
 */

#ifndef FL_UI_TEXTURE_ELEMENT_H_
#define FL_UI_TEXTURE_ELEMENT_H_

#include "common/basic_types.h"
#include "components/texture/fl_texture.h"
#include "fl_ui_basic_element.h"

class FLUITextureElement : public FLUIBasicElement {
	public:
		FLUITextureElement(FLTextureSurface *surface);
		virtual ~FLUITextureElement();

		virtual void render();

	private:
		FLTexture* texture;
		FLTextureSurface *surface;
};

#endif

