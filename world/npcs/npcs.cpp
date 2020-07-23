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
}

FLNpc::~FLNpc() {
	Renderer::getInstance().remove_from_world(animators["body"]);
}

void FLNpc::interact_with() {
	std::cout << "ummmm hi" << std::endl;
	/*
	set_game_state( FL_GAME_UI );
	float cx = Renderer::getInstance().world_camera_x() / -2.f;
	float cy = Renderer::getInstance().world_camera_y() / -2.f;
	float _y = y() - cy + (512.f / 2.f);
	float _x = x() - cx + (768.f / 2.f);
	new FLDialogueBox( "", _x, _y );
	*/
}

