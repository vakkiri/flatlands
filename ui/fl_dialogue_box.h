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

class FLGameObject;

struct fl_message {
	std::string text;
	FLGameObject* speaker;
	bool flipped;
};

class FLDialogueBox : public FLUIElement {
  public:
	FLDialogueBox() = delete;
	FLDialogueBox(std::vector<fl_message> text);

	virtual void accept() override;
	std::vector<int>& get_textured_objects() override;
  protected:
	virtual void init_textures();

	unsigned int width;
	unsigned int height;
	unsigned int border_size;

	int bg_id;
	int player_portrait_id;
	int npc_portrait_id;

	std::vector<fl_message> messages;
};

#endif
