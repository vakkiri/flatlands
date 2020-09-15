/*
 * 	fl_teleporter_box.cpp
 *
 */

#include <iostream>

#include "../common/game_object.h"
#include "../rendering/renderer.h"
#include "../rendering/textured_object.h"
#include "fl_teleporter_box.h"
#include "fl_ui_manager.h"

#define W 16
#define H 16

FLTeleporterBox::FLTeleporterBox(float x, float y, std::vector<int>& area_ids)
	: FLUIElement(0, 0) {
	this->x = x;
	this->y = y;
	this->area_id = area_id;
	this->area_ids = area_ids;

	width = 16;
	height = 16;

	init_textures();

	FLUIManager::getInstance().add_element(this);
	FLUIManager::getInstance().set_active_element(this);

	active_id = 0;
}

void FLTeleporterBox::accept() {
	// TODO: trigger level load/teleportation animation
	FLUIManager::getInstance().remove_element(this);
	delete this;
}

void FLTeleporterBox::reject() {
	FLUIManager::getInstance().remove_element(this);
	delete this;
}

void FLTeleporterBox::left() {
	// lol @ using unsigned integers...
	if (active_id-- == 0) {
		active_id = area_ids.size() - 1;
	}
}

void FLTeleporterBox::right() {
	if (++active_id >= area_ids.size()) {
		active_id = 0;
	}
}

void FLTeleporterBox::init_textures() {
	for (auto id : area_ids) { 
		FLTexturedObject* tex = new FLTexturedObject(x, y, W, H);
		tex->set_st(16 * id, 96);
		
		textured_objects.push_back(tex);
	}
}

std::vector<FLTexturedObject*>& FLTeleporterBox::get_textured_objects() {
	point p = Renderer::getInstance().screen_pos(x, y);
	for (unsigned int i = 0; i < textured_objects.size(); ++i) {
		textured_objects[i]->set_x(p.x - 32.f + (32.f * i));
		textured_objects[i]->set_y(p.y - 32.f);
		if (i == active_id) {
			textured_objects[i]->set_st(16 * area_ids[i], 112);
		} else {
			textured_objects[i]->set_st(16 * area_ids[i], 96);
		}
	}

	return FLUIElement::get_textured_objects();
}

