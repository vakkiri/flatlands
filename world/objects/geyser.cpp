/*
 * 	geyser.cpp
 *
 */

#include <vector>
#include <iostream>

#include "geyser.h"
#include "common/fl_object.h"
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
#define ACTIVE_TICKS FRAMES * FRAME_TICKS * 2

namespace FLGeysers {
	std::vector<fl_handle> geysers;

	void clear() {
		while (!geysers.empty()) {
			FLObjects::destroy(geysers.back());
			geysers.pop_back();
		}
	}

	void update(FLObject& geyser) {
		int tick = std::get<int>(geyser.vars["tick"]);
		tick += 1;

		if (geyser.states["state"] == FL_GEYSER_IDLE) {
			if (tick >= IDLE_TICKS) {
				tick = 0;
				geyser.states["state"] = FL_GEYSER_ACTIVE;
				FLObjects::set_animation(geyser.handle, "geyser", "geyser_active");
			}
		} else {
			if (tick >= ACTIVE_TICKS) {
				tick = 0;
				geyser.states["state"] = FL_GEYSER_IDLE;
				FLObjects::set_animation(geyser.handle, "geyser", "geyser_idle");

				// TODO: port effect and aoeburst
				float x = FLObjects::x(geyser.handle);
				float y = FLObjects::y(geyser.handle);
				FLTexturedObjectParams tex_params = {nullptr, x, y - 48, 32, 48};
				FLAnimatedObjectParams anim_params = {1, 8, 4, 32, 48, false};
				new FLEffect(tex_params, anim_params, 544, 80);
				new FLAoeBurst(x + 4, y - 24, 24, 24, 25, true);

			}
		}

		geyser.vars["tick"] = tick;
	}

	void init(fl_handle handle, float x, float y) {
		std::string tex_name = "geyser";
		std::string ani_name = "geyser";
		std::string surface_name = "world";
		FLObjects::set_pos(handle, x, y);
		FLObjects::add_texture(handle, tex_name, surface_name, "geyser_idle");
		FLObjects::add_animator(handle, ani_name, tex_name, "geyser_idle", 4);
		FLObjects::add_var(handle, "tick", 0);
		FLObjects::add_state(handle, "state", FL_GEYSER_IDLE);
		FLObjects::add_script(handle, update);
	}

	void create(float x, float y) {
		fl_handle new_geyser = FLObjects::create();

		if (new_geyser != NULL_HANDLE) {
			init(new_geyser, x, y);
		} else {
			std::cout << "Warning: could not create geyser.\n";
		}
	}
}

/*
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
*/
