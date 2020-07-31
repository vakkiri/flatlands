/*
 * 	fl_dialogue_box.cpp
 *
 */

#include <iostream>

#include "../rendering/renderer.h"
#include "../rendering/fl_text_surface.h"
#include "../rendering/textured_object.h"
#include "fl_dialogue_box.h"
#include "fl_ui_manager.h"

#define S
#define T

FLDialogueBox::FLDialogueBox() : FLDialogueBox("", 0, 0) {}

FLDialogueBox::FLDialogueBox(std::string text, float x, float y)
	: FLUIElement(x, y) {
	messages.push_back(text);
	width = 192;
	height = 64;
	border_size = 7;

	init_textures();

	FLUIManager::getInstance().add_element(this);
	FLUIManager::getInstance().set_active_element(this);
}

void FLDialogueBox::accept() {
	messages.pop_back();

	if (messages.empty()) {
		FLUIManager::getInstance().remove_element(this);
		delete this;
	} else {
	}
}

void FLDialogueBox::init_textures() {
	background = new FLTexturedObject(offset.x, offset.y, width, height);
	background->set_st(0.f, 32.f);
	
	textured_objects.push_back(background);
}

// TODO: this should be replaced with a render() function instead of returning verts wtf lmao
// i see why i thought this may have been a good idea
// but it was quite a bad idea

std::vector<FLTexturedObject*>& FLDialogueBox::get_textured_objects() {
	FLTextSurface* text_surface = Renderer::getInstance().get_text_surface();
	float left = offset.x + border_size;
	float right = offset.x + width - border_size - 5;
	float x = offset.x + border_size;
	float y = offset.y + border_size;
	float charw = 5;	// this should be grabbed from font lol

	std::string msg = messages.front();

	unsigned int i = 0;
	while (i < msg.size()) {
		unsigned int cur = i;
		unsigned int end = i;
		float curx = left;
		x = left;

		for (cur = i; cur < msg.size(); ++cur) {
			if (msg[cur] == ' ') {
				end = cur;
			}
			curx += charw;
			if (curx >= right) {
				break;
			}
		}

		// if we hit the end of the string
		if (cur >= msg.size() || msg[cur] == ' ') {
			end = cur;
		}

		while (i <= end) {
			text_surface->add_character(x, y, msg[i]);
			x += charw;
			++i;
		}

		y += 12;
	}

	return FLUIElement::get_textured_objects();
}
