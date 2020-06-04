/*
 * 	fl_ammobar.h
 *
 * 	A UI element for the player's ammo.
 *
 */

#ifndef FL_AMMOBAR_H_
#define FL_AMMOBAR_H_

#include "fl_ui_element.h"

class FLAmmobar : public FLUIElement {
  public:
	FLAmmobar();
	~FLAmmobar();

	virtual std::vector<std::vector<fl_colored_vertex>> &
	get_primitive_vertices();

  protected:
	virtual void init_textures();
	virtual void init_vertices();

	FLTexturedObject *icon;
	FLTexturedObject *bar_image;
};

#endif
