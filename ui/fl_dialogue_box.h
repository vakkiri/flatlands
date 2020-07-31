/*
 * 	fl_dialogue_box.h
 *
 * 	UI element for drawing boxes with text dialogue inside.
 *
 */

#ifndef FL_DIALOGUE_BOX_H_
#define FL_DIALOGUE_BOX_H_

#include <string>
#include <vector>

#include "fl_ui_element.h"

class FLDialogueBox : public FLUIElement {
  public:
	FLDialogueBox();
	FLDialogueBox(std::string text, float x, float y);

	virtual void accept();
	std::vector<FLTexturedObject*>& get_textured_objects() override;
  protected:
	virtual void init_textures();

	unsigned int width;
	unsigned int height;
	unsigned int border_size;

	FLTexturedObject* background;
	std::vector<std::string> messages;
};

#endif
