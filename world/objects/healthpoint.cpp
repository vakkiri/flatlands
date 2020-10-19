/*
 * 	healthpoint.cpp
 *
 */

#include <iostream>
#include "healthpoint.h"
#include "../player/player.h"
#include "../../components/components.h"
#include "../../environment/fl_environment.h"
#include "../../game/fl_game.h"
#include "../../rendering/animated_object.h"
#include "../../rendering/renderer.h"

#define NUM_ANIMATIONS 	1
#define NUM_STEPS 	4
#define FRAMES_PER_STEP	4
#define REPEATS		true

#define W 16
#define H 48
#define S 624
#define T 16

static std::vector<FLHealthPoint*> healthpoints;

static FLAnimatedObjectParams animation_params = {
	NUM_ANIMATIONS,
	NUM_STEPS,
	FRAMES_PER_STEP,
	W,
	H,
	REPEATS
};

FLHealthPoint::FLHealthPoint(float x, float y) : FLGameObject(x, y, W, H) {
	FLTexturedObjectParams tex_params = {shapes["position"], 0, 0, W, H};
	animators["body"] = new FLAnimatedObject(tex_params, animation_params);
	Renderer::getInstance().add_to_world(animators["body"]);
	animators["body"]->set_st(S, T);
	interactable_handle = fl_new_interactable(this);

	healthpoints.push_back(this);
}

FLHealthPoint::~FLHealthPoint() {
	fl_delete_interactable(interactable_handle);
	Renderer::getInstance().remove_from_world(animators["body"]);
}

void FLHealthPoint::interact() {
	FLGame::instance().environment()->player()->heal();
}

void clear_healthpoints() {
	while (!healthpoints.empty()) {
		delete healthpoints.back();
		healthpoints.pop_back();
	}
}

