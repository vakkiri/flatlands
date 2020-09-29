/*
 * 	water.cpp
 *
 */

#include <vector>
#include <iostream>

#include "water.h"
#include "../rendering/renderer.h"
#include "../rendering/water_renderable.h"

static std::vector<FLWater*> water;

FLWater::FLWater(float x, float y, float w, float h) : FLGameObject(x, y, w, h) {
	renderers["body"] = new FLWaterSurface(x, y, w, h);
	Renderer::getInstance().add_custom(renderers["body"]);
	water.push_back(this);
}

FLWater::~FLWater() {
	Renderer::getInstance().remove_custom(renderers["body"]);
}

void clear_water() {
	while (!water.empty()) {
		delete water.back();
		water.pop_back();
	}
}

