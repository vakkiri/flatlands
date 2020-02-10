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

class FLDialogueBox : public FLUIElement {
	public:
		FLDialogueBox(std::string text);

		virtual void accept();
		virtual void render();
	private:
		std::vector<std::string> messages;
};


#endif

