/*
 * 	scenery.cpp
 *
 */

#include <iostream>

#include "scenery.h"
#include "../rendering/renderer.h"

FLScenery::FLScenery(float x, float y, int index) 
	: FLGameObject(x, y, 1, 1) {
		float s, t, w, h;
		// FIXME this is stupid and should be read from a file, and then
		// grabbed from the resources class lol
		if (index == 101) {
			s = 0;
			t = 0;
			w = 98;
			h = 88;
		} else if (index == 102) {
			s = 99;
			t = 0;
			w = 35;
			h = 27;
		} else if (index == 103) {
			s = 0;
			t = 96;
			w = 245;
			h = 68;
		} else {
			std::cout << "Warning: unknown scenery <" << index << ">" << std::endl;
		}

		FLTexturedObjectParams tex_params = {shapes["position"], 0, 0, w, h};
		FLAnimatedObjectParams animation_params = {1, 1, 1, 0, 0, false};
		animators["body"] = new FLAnimatedObject(tex_params, animation_params);
		animators["body"]->set_st(s, t);
		Renderer::getInstance().add_to_world(animators["body"], true);
}

FLScenery::~FLScenery() {
	Renderer::getInstance().remove_from_world(animators["body"]);
}

