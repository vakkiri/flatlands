/*
 * 	telepoerter.cpp
 *
 */

#include <iostream>
#include "teleporter.h"
#include "../components/components.h"
#include "../environment/fl_environment.h"
#include "../game/fl_game.h"
#include "../rendering/animated_object.h"
#include "../rendering/renderer.h"
#include "../ui/fl_teleporter_box.h"
#include "../game/fl_gamestate.h"
#include "player/player.h"

#define TP_WIDTH 16
#define TP_HEIGHT 16
static std::vector<FLTeleporter*> teleporters;

FLTeleporter::FLTeleporter(float x, float y)
	: FLGameObject(x, y, TP_WIDTH, TP_HEIGHT) {
	interactable_handle = fl_new_interactable(this);

	FLTexturedObjectParams tex_params = {shapes["position"], 0, 0, TP_WIDTH, TP_HEIGHT};
	FLAnimatedObjectParams animation_params = {1, 3, 5, 16, 0, true};
	animators["body"] = new FLAnimatedObject(tex_params, animation_params);
	animators["body"]->set_st(672, 0);
	Renderer::getInstance().add_to_world(animators["body"], false);

	teleporters.push_back(this);
}

FLTeleporter::~FLTeleporter() {
	Renderer::getInstance().remove_from_world(animators["body"]);
}

void FLTeleporter::interact() {
	set_game_state(FL_GAME_UI);
	new FLTeleporterBox(x(), y(), 0);
	// create a teleporter UI -> this will handle actually teleporting
}

void clear_teleporters() {
	while (!teleporters.empty()) {
		delete teleporters.back();
		teleporters.pop_back();
	}
}

