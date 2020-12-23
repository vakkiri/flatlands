/*
 * 	background.cpp
 *
 */

#include <iostream>
#include <unordered_map>
#include "background.h"

static std::unordered_map<std::string, FLBackground> backgrounds;

void add_background(std::string name) {
	if (backgrounds.find(name) != backgrounds.end()) {
		std::cout << "Error: duplicate background " << name << "\n";
	}

	backgrounds[name] = FLBackground();
}

void add_background_layer(std::string name, std::string tex, float px, float py) {
	if (backgrounds.find(name) == backgrounds.end()) {
		std::cout << "Error: could not find background " << name << "\n";
	} else {
		backgrounds[name].add_layer(tex, px, py);
	}
}

std::vector<bg_layer>& get_background_layers(std::string name) {
	return backgrounds[name].get_layers();
}

void FLBackground::add_layer(std::string tex, float px, float py) {
	bg_layer l = {tex, px, py};
	layers.push_back(l);
}

std::vector<bg_layer>& FLBackground::get_layers() {
	return layers;
}

