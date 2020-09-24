/*
 * 	fl_chips.h
 *
 * 	A UI element for the player's chips.
 *
 */

#ifndef FL_CHIPS_H_
#define FL_CHIPS_H_

#include "fl_ui_element.h"

class FLChipbar : public FLUIElement {
  public:
	FLChipbar();

	std::vector<FLTexturedObject*> &get_textured_objects() override;
  protected:
	virtual void init_textures();

	FLTexturedObject *icon;
	FLTexturedObject *fragment_icon;
};

#endif
