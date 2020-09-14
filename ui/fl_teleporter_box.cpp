/*
 * 	fl_teleporter_box.cpp
 *
 */

#include <iostream>

#include "../common/game_object.h"
#include "../rendering/renderer.h"
#include "../rendering/fl_text_surface.h"
#include "../rendering/textured_object.h"
#include "fl_teleporter_box.h"
#include "fl_ui_manager.h"

#define W 16
#define H 16

FLTeleporterBox::FLTeleporterBox(int area_id)
	: FLUIElement(0, 0) {

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
	tex = new FLTexturedObject(0, 0, W, H);
	tex->set_st(s, t);
	
	textured_objects.push_back(tex);
}


