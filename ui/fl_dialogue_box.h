/*
 * 	fl_dialogue_box.h
 *
 * 	UI element for drawing boxes with text dialogue inside.
 *
 */

#ifndef FL_DIALOGUE_BOX_H_
#define FL_DIALOGUE_BOX_H_

#include <vector>
#include <string>

#include "fl_ui_element.h"

class FLDialogueBox : public FLUIElement {
	public:
		FLDialogueBox();
		FLDialogueBox(std::string text);

		virtual void accept();
	protected:
		unsigned int width;
		unsigned int height;
		unsigned int border_size;
		std::vector<std::string> messages;
};


#endif

