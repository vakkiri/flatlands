/*
 * 	npcs.cpp
 *
 */

#include <fstream>
#include <iostream>

#include "npcs.h"

#include "../player/player.h"
#include "../../environment/fl_environment.h"
#include "../../game/fl_gamestate.h"
#include "../../game/fl_game.h"
#include "../../rendering/renderer.h"
#include "../../ui/fl_dialogue_box.h"
#include "../../ui/fl_ui_manager.h"
#include "../../components/components.h"

static std::vector<FLNpc*> npcs;

FLNpc::FLNpc(float x, float y, float w, float h,
		FLAnimatedObjectParams animation_params)
	: FLGameObject(x, y, w, h) {
	FLTexturedObjectParams tex_params = {shapes["position"], 0, 0, w, h};
	animators["body"] = new FLAnimatedObject(tex_params, animation_params);
	Renderer::getInstance().add_to_world(animators["body"]);
	animators["body"]->set_st(912, 96);
	interactable_handle = fl_new_interactable(this);

	npcs.push_back(this);
}

FLNpc::~FLNpc() {
	fl_delete_interactable(interactable_handle);
	Renderer::getInstance().remove_from_world(animators["body"]);
}

void FLNpc::interact() {
	FLPlayer *player = FLGame::instance().environment()->player();
	bool flipped = player->x() > x();
	set_game_state( FL_GAME_UI );

	std::vector<fl_message> messages;
	std::ifstream file("assets/text/0");
	std::string line;
	int state = 0;	// 0 = get speaker 1 = get text
	int speaker = 0; // 0 = player, 1 = npc

	while (std::getline(file, line)) { 
		if (line.size() > 0 && line != "\n") {
			if (state == 0) {
				if (line == "player") {
					speaker = 0;
				} else {
					speaker = 1;
				}
				state= 1;
			} else {
				if (speaker == 0) {
					messages.push_back(fl_message{line, player, flipped});
				} else {
					messages.push_back(fl_message{line, this, !flipped});
				}
				state = 0;
			} 
		}
	}

	new FLDialogueBox(messages);
}

void clear_npcs() {
	while (!npcs.empty()) {
		delete npcs.back();
		npcs.pop_back();
	}
}

