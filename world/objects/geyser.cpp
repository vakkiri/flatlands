/*
 * 	geyser.cpp
 *
 */

#include <vector>
#include <iostream>

#include "geyser.h"
#include "../effect.h"
#include "../projectiles/fl_projectiles.h"
#include "../../components/components.h"
#include "../../rendering/renderer.h"


#define W 32
#define H 16
#define S 544
#define T 64
#define FRAMES 8
#define FRAME_TICKS 4

#define IDLE_TICKS 120
#define PRE_TICKS FRAMES * FRAME_TICKS * 2

static std::vector<FLGeyser*> geysers;

FLGeyser::FLGeyser(float x, float y) : FLGameObject(x, y, W, H) {
	FLTexturedObjectParams tex_params = {shapes["position"], 0, 0, W, H};
	FLAnimatedObjectParams animation_params = {1, FRAMES, FRAME_TICKS, W, H, true};
	animators["body"] = new FLAnimatedObject(tex_params, animation_params);
	animators["body"]->set_st(S, T);
	Renderer::getInstance().add_to_world(animators["body"], false);

	updator_handle = new_updator(this);

	geysers.push_back(this);
	tick = 0;
	state = FL_GEYSER_IDLE;
}

FLGeyser::~FLGeyser() {
	Renderer::getInstance().remove_from_world(animators["body"]);
}

void FLGeyser::per_frame_update() {
	++tick;

	if (state == FL_GEYSER_IDLE) {
		if (tick >= IDLE_TICKS) {
			animators["body"]->reset_animation();
			animators["body"]->start_animation();
			tick = 0;
			state = FL_GEYSER_PRE_BURST;
		}
	} else {
		if (tick >= PRE_TICKS) {
			FLTexturedObjectParams tex_params = {nullptr, x(), y() - h() - 16, 32, 48};
			FLAnimatedObjectParams anim_params = {1, 8, 4, 32, 48, false};
			new FLEffect(tex_params, anim_params, 544, 80);
			new FLAoeBurst(x() + 4, y() - 24, 24, 24, 25, true);

			animators["body"]->reset_animation();
			animators["body"]->stop_animation();
			tick = 0;
			state = FL_GEYSER_IDLE;
		}
	}
}

void clear_geysers() {
	while (!geysers.empty()) {
		delete geysers.back();
		geysers.pop_back();
	}
}

