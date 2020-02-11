/*
 * 	fl_dialogue_box.cpp
 *
 */

#include "fl_dialogue_box.h"

FLDialogueBox::FLDialogueBox() : FLDialogueBox("") {}

FLDialogueBox::FLDialogueBox(std::string text) {
	// TODO: process text into single-box sized messages
	messages.push_back(text);
	// TODO: set appropriate text widths
	width = 512;
	height = 256;
	border_size = 4;
}

void FLDialogueBox::accept() {
	messages.pop_back();

	if (messages.empty()) {
	}
	else {
	}
}

