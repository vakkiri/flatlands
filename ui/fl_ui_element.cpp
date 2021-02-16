/*
 * 	fl_ui_element.cpp
 *
 */

#include <iostream>

#include "../rendering/textured_object.h"
#include "fl_ui_element.h"

FLUIElement::FLUIElement() : FLUIElement(0, 0) {}

FLUIElement::FLUIElement(float x, float y) {
	offset.x = x;
	offset.y = y;
}

FLUIElement::~FLUIElement() {
	while (!textured_objects.empty()) {
		delete_texturer(textured_objects.back());
		textured_objects.pop_back();
	}
}

std::vector<std::vector<fl_colored_vertex>> &
FLUIElement::get_primitive_vertices() {
	return primitive_vertices;
}

std::vector<int> &FLUIElement::get_textured_objects() {
	return textured_objects;
}

void FLUIElement::set_pos(float x, float y) {
	offset.x = x;
	offset.y = y;
}

