/*
 * 	water.cpp
 *
 */

#include <vector>
#include <iostream>

#include "water.h"

static std::vector<FLWater*> water;

FLWater::FLWater(float x, float y, float w, float h) : FLGameObject(x, y, w, h) {
}

FLWater::~FLWater() {}

void clear_water() {
	while (!water.empty()) {
		delete water.back();
		water.pop_back();
	}
}

