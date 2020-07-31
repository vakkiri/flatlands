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
	std::string get_message();
	std::vector<std::vector<fl_colored_vertex>>& get_primitive_vertices() override;
  protected:
	// protected methods
	virtual void init_vertices();

	// member variables
	unsigned int width;
	unsigned int height;
	unsigned int border_size;

	std::vector<std::string> messages;
};

#endif
