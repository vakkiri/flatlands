/*
 * 	fl_ui_group.cpp
 *
 */

#include <iostream>
#include "fl_ui_basic_element.h"
#include "fl_ui_vector_element.h"
#include "fl_ui_texture_element.h"
#include "fl_ui_text_element.h"
#include "fl_ui_group.h"
#include "fl_ui.h"

FLUIGroup::FLUIGroup(std::string name) : 
	active_element(nullptr),
	name(name) {}

FLUIGroup::~FLUIGroup() {
	while (!vectors.empty()) {
		delete vectors.back();
		vectors.pop_back();
	}
	while (!textures.empty()) {
		delete textures.back();
		textures.pop_back();
	}
	while (!text.empty()) {
		delete text.back();
		text.pop_back();
	}
}

void FLUIGroup::add_vector(std::string name, FLUIVectorElement *vec) {
	if (elements.find(name) == elements.end()) {
		elements[name] = vec;
		vectors.push_back(vec);
	} else {
		std::cout << "Error: " << name << " already exists in UI Group " << this->name << std::endl;
	}
}

void FLUIGroup::add_texture(std::string name, FLUITextureElement *tex) {
	if (elements.find(name) == elements.end()) {
		elements[name] = tex;
		textures.push_back(tex);
	} else {
		std::cout << "Error: " << name << " already exists in UI Group " << this->name << std::endl;
	}
}

void FLUIGroup::add_text(std::string name, FLUITextElement *text) {
	if (elements.find(name) == elements.end()) {
		elements[name] = text;
		this->text.push_back(text);
	} else {
		std::cout << "Error: " << name << " already exists in UI Group " << this->name << std::endl;
	}
}

void FLUIGroup::set_active_element(FLUIBasicElement *element) {
	active_element = element;
}

void FLUIGroup::render() {
	for (auto [name, element] : elements) {
		element->render();
	}

	vector_surface.render();
	texture_surface.render();
	text_surface.render();
}

void FLUIGroup::handle_up() {
	if (active_element) {
		active_element = active_element->up();
	}
}

void FLUIGroup::handle_down() {
	if (active_element) {
		active_element = active_element->down();
	}
}

void FLUIGroup::handle_left() {
	if (active_element) {
		active_element = active_element->left();
	}
}

void FLUIGroup::handle_right() {
	if (active_element) {
		active_element = active_element->right();
	}
}

void FLUIGroup::handle_accept() {
	if (active_element) {
		active_element->accept();
	}
}

void FLUIGroup::handle_reject() {
	FLUI::remove_group(name);
}

void FLUIGroup::update() {
	for (auto a : vectors) {
		a->update();
	}
	for (auto a : textures) {
		a->update();
	}
	for (auto a : text) {
		a->update();
	}
}

