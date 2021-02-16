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

	std::vector<int> &get_textured_objects() override;
  protected:
	virtual void init_textures();

	int icon_id;
	int fragment_icon_id;
};

#endif
