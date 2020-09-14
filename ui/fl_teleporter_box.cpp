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

FLTeleporterBox::FLTeleporterBox(float x, float y, int area_id)
	: FLUIElement(0, 0) {
	this->x = x;
	this->y = y;
	this->area_id = area_id;

	width = 16;
	height = 16;
	s = 16 * area_id;
	t = 96;

	init_textures();

	FLUIManager::getInstance().add_element(this);
	FLUIManager::getInstance().set_active_element(this);
}

void FLTeleporterBox::accept() {
	// TODO: trigger level load/teleportation animation
	FLUIManager::getInstance().remove_element(this);
	delete this;
}

void FLTeleporterBox::reject() {
	// TODO: clear all other boxes, t
	FLUIManager::getInstance().remove_element(this);
	delete this;
}

void FLTeleporterBox::init_textures() {
	tex = new FLTexturedObject(x, y, W, H);
	tex->set_st(s, t);
	
	textured_objects.push_back(tex);
}

std::vector<FLTexturedObject*>& FLTeleporterBox::get_textured_objects() {
	point p = Renderer::getInstance().screen_pos(x, y);
	tex->set_x(p.x - 32.f);
	tex->set_y(p.y - 32.f);

	return FLUIElement::get_textured_objects();
}

