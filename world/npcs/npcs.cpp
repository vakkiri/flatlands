/*
 * 	npcs.cpp
 *
 */

#include <iostream>

#include "npcs.h"

#include "../../ui/fl_ui_manager.h"
#include "../../ui/fl_dialogue_box.h"
#include "../../game/fl_gamestate.h"
#include "../../rendering/renderer.h"

#define NUM_ANIMATIONS 1
#define NUM_STEPS 2
#define FRAMES_PER_STEP 10
#define REPEATS true

FLNpc::FLNpc( float x, float y, float s, float t, float w, float h ) : FLGameObject(x, y, w, h ) {
	//set_st( s, t );
	//Renderer::getInstance().add_to_world( this );
}

FLNpc::~FLNpc() {
	//Renderer::getInstance().remove_from_world( this );
}

void FLNpc::interact_with() {
	/*
	set_game_state( FL_GAME_UI );
	float cx = Renderer::getInstance().world_camera_x() / -2.f;
	float cy = Renderer::getInstance().world_camera_y() / -2.f;
	float _y = y() - cy + (512.f / 2.f);
	float _x = x() - cx + (768.f / 2.f);
	new FLDialogueBox( "", _x, _y );
	*/
}

