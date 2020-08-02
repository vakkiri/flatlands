/*
 * 	fl_dialogue_box.cpp
 *
 */

#include <iostream>

#include "../common/game_object.h"
#include "../rendering/renderer.h"
#include "../rendering/fl_text_surface.h"
#include "../rendering/textured_object.h"
#include "fl_dialogue_box.h"
#include "fl_ui_manager.h"

#define S
#define T

FLDialogueBox::FLDialogueBox(std::vector<fl_message> text)
	: FLUIElement(0, 0) {

	while (!text.empty()) {	
		messages.push_back(text.back());
		text.pop_back();
	}

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
	Renderer &r = Renderer::getInstance();
	FLTextSurface* text_surface = r.get_text_surface();

	fl_message msg = messages.back();
	float cx = r.world_camera_x() / -2.f;
	float cy = r.world_camera_y() / -2.f;
	float _x = msg.speaker->x() - cx + r.get_screen_width() / 4.f;
	float _y = msg.speaker->y() - height - cy + r.get_screen_height() / 4.f;
	if (!msg.flipped) {
		_x -= width;
	} else {
		_x += msg.speaker->w();
	}

	set_pos(_x, _y);

	background->set_x(_x);
	background->set_y(_y);
	background->set_reverse(msg.flipped);

	float left = offset.x + border_size;
	float right = offset.x + width - border_size - 5;
	float x = offset.x + border_size;
	float y = offset.y + border_size;
	float charw = 5;	// this should be grabbed from font lol

	unsigned int i = 0;
	while (i < msg.text.size()) {
		unsigned int cur = i;
		unsigned int end = i;
		float curx = left;
		x = left;

		for (cur = i; cur < msg.text.size(); ++cur) {
			if (msg.text[cur] == ' ') {
				end = cur;
			}
			curx += charw;
			if (curx >= right) {
				break;
			}
		}

		// if we hit the end of the string
		if (cur >= msg.text.size() || msg.text[cur] == ' ') {
			end = cur;
		}

		while (i <= end) {
			text_surface->add_character(x, y, msg.text[i]);
			x += charw;
			++i;
		}

		y += 12;
	}

	return FLUIElement::get_textured_objects();
}
