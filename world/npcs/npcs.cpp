/*
 * 	npcs.cpp
 *
 */

#include <iostream>

#include "npcs.h"

#include "../../game/fl_gamestate.h"
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
	Renderer& r = Renderer::getInstance();

	set_game_state( FL_GAME_UI );
	float cx = r.world_camera_x() / -2.f;
	float cy = r.world_camera_y() / -2.f;
	float _y = y() - cy + r.get_screen_height() / 4.f;
	float _x = x() - cx + r.get_screen_width() / 4.f;
	_y -= 64.f;
	_x -= 210.f;

	std::string test_string = "Quick wafting zephyrs vex bold Jim. Truly wonderful.";
	new FLDialogueBox(test_string, _x, _y);
}

