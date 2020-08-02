/*
 * 	npcs.cpp
 *
 */

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
#define NUM_ANIMATIONS 1
#define NUM_STEPS 2
#define FRAMES_PER_STEP 10
#define REPEATS true

FLNpc::FLNpc(float x, float y, float w, float h,
		FLAnimatedObjectParams animation_params)
	: FLGameObject(x, y, w, h) {
	FLTexturedObjectParams tex_params = {shapes["position"], 0, 0, w, h};
	animators["body"] = new FLAnimatedObject(tex_params, animation_params);
	Renderer::getInstance().add_to_world(animators["body"]);
	animators["body"]->set_st(912, 96);
	interactable_handle = fl_new_interactable(this);
}

FLNpc::~FLNpc() {
	Renderer::getInstance().remove_from_world(animators["body"]);
}

void FLNpc::interact() {
	FLPlayer *player = FLGame::instance().environment()->player();
	bool flipped = player->x() > x();
	set_game_state( FL_GAME_UI );

	std::string test_string = "Quick wafting zephyrs vex bold Jim. Truly wonderful.";
	std::vector<fl_message> messages;
	messages.push_back(fl_message{"Hey guy!", player, flipped});
	messages.push_back(fl_message{"Yeah?", this, !flipped});
	messages.push_back(fl_message{"Ummm nothing.", player, flipped});
	new FLDialogueBox(messages);
}

