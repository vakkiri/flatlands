/*
 * 	fl_dialogue_box.cpp
 *
 */

#include <iostream>

#include "../common/game_object.h"
#include "../environment/fl_environment.h"
#include "../game/fl_game.h"
#include "../rendering/renderer.h"
#include "../rendering/fl_camera.h"
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

	width = 208;
	height = 48;
	border_size = 12;

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
	bg_id = new_texturer();
	player_portrait_id = new_texturer();
	npc_portrait_id = new_texturer();

	if (bg_id > 0) {
		FLTexturedObject *bg = get_texturer(bg_id);
		bg->set_x(offset.x);
		bg->set_y(offset.y);
		bg->set_w(width);
		bg->set_h(height);
		bg->set_st(0.f, 32.f);
		textured_objects.push_back(bg_id);
	}
	if (player_portrait_id > 0) {
		FLTexturedObject* p = get_texturer(player_portrait_id);
		p->set_x(offset.x + 8);
		p->set_y(offset.y + 8);
		p->set_w(31);
		p->set_h(37);
		p->set_st(0.f, 128.f);
		textured_objects.push_back(player_portrait_id);
	}
	if (npc_portrait_id > 0) {
		FLTexturedObject* npc = get_texturer(npc_portrait_id);
		npc->set_x(offset.x + 8);
		npc->set_y(offset.y + 8);
		npc->set_w(31);
		npc->set_h(37);
		npc->set_st(32.f, 128.f);
		textured_objects.push_back(npc_portrait_id);
	}


}

// TODO: this should be replaced with a render() function instead of returning verts wtf lmao
// i see why i thought this may have been a good idea
// but it was quite a bad idea

std::vector<int>& FLDialogueBox::get_textured_objects() {
	Renderer &r = Renderer::getInstance();
	FLTextSurface* text_surface = r.get_text_surface();

	FLTexturedObject* background = get_texturer(bg_id);
	FLTexturedObject* player_portrait = get_texturer(player_portrait_id);
	FLTexturedObject* npc_portrait = get_texturer(npc_portrait_id);

	fl_message msg = messages.back();

	// calculate the screen position (not world position) of the speaker by
	// subtracting the camera position
	float cx = r.get_world_camera()->x() / -r.get_world_camera()->x_scale();
	float cy = r.get_world_camera()->y() / -r.get_world_camera()->y_scale();
	float _x = msg.speaker->x() - width - cx;
	float _y = msg.speaker->y() - height - cy;

	set_pos(_x, _y);

	background->set_x(_x);
	background->set_y(_y);

	player_portrait->set_x(_x + 8);
	player_portrait->set_y(_y + 6);

	npc_portrait->set_x(_x + 8);
	npc_portrait->set_y(_y + 6);

	player_portrait->set_visible(msg.speaker == (FLGameObject*) FLGame::instance().environment()->player());
	npc_portrait->set_visible(msg.speaker != (FLGameObject*) FLGame::instance().environment()->player());

	float left = offset.x + border_size + 52;
	float right = offset.x + width - border_size;
	float x = offset.x + border_size;
	float y = offset.y + 6;
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
