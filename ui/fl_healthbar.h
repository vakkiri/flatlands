/*
 * 	fl_healthbar.h
 *
 * 	A UI element for the player's healthbar.
 *
 */

#ifndef FL_HEALTHBAR_H_
#define FL_HEALTHBAR_H_

#include "fl_ui_element.h"

class FLHealthbar : public FLUIElement {
	public:
		FLHealthbar();
		~FLHealthbar();

		virtual std::vector<std::vector<fl_colored_vertex>>& get_primitive_vertices();
	protected:
		virtual void init_textures();
		virtual void init_vertices();
		FLTexturedObject* icon;
		FLTexturedObject* bar_image;
};

#endif

