/*
 * 	npcs.cpp
 *
 */

#include <iostream>

#include "npcs.h"
#include "../../game/fl_gamestate.h"
#include "../../rendering/renderer.h"
#include "../world_environment.h"

#define NUM_ANIMATIONS 1
#define NUM_STEPS 2
#define FRAMES_PER_STEP 10
#define REPEATS true

FLNpc::FLNpc( float x, float y, float s, float t, float w, float h ) :
	FLGameObject(x, y, w, h ),
	FLAnimatedObject(
			NUM_ANIMATIONS,
			NUM_STEPS,
			FRAMES_PER_STEP,
			w,
			REPEATS
			) {
	set_st( s, t );
	Renderer::getInstance().add_to_world( this );
	FLWorldEnvironment::getInstance().add_interactable_object( this );
}

FLNpc::~FLNpc() {
	Renderer::getInstance().remove_animated_object( this );
	Renderer::getInstance().remove_from_world( this );
	FLWorldEnvironment::getInstance().remove_interactable_object( this );
}

void FLNpc::interact_with() {
	set_game_state( FL_GAME_UI );
}

