/*
 * 	portal.cpp
 *
 */

#include <iostream>
#include "portal.h"
#include "../components/components.h"
#include "../environment/fl_environment.h"
#include "../game/fl_game.h"
#include "player/player.h"

static std::vector<FLPortal*> portals;

FLPortal::FLPortal(float x, float y, float w, float h, float destx, float desty, int dest_level) 
	: FLGameObject(x, y, w, h) {
	this->destx = destx;
	this->desty = desty;
	this->dest_level = dest_level;
	interactable_handle = fl_new_interactable(this);
	portals.push_back(this);
}

FLPortal::~FLPortal() {
	fl_delete_interactable(interactable_handle);
}

void FLPortal::interact() {
	FLEnvironment *env = FLGame::instance().environment();
	FLPlayer *player = env->player();
	// case 1: the portal leads to another area on the same level
	// in this case, we just set x and y
	// case 2: load a different level
	if (env->get_level_id() == dest_level) {
		player->set_x(destx);
		player->set_y(desty);
	} else {
		env->load_level(dest_level, destx, desty);
	}
}

void clear_portals() {
	while (!portals.empty()) {
		delete portals.back();
		portals.pop_back();
	}
}

