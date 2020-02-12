/*
 * 	fl_ui_element.cpp
 *
 */

#include <iostream>

#include "fl_ui_element.h"

FLUIElement::FLUIElement() : FLUIElement(0, 0) {}

FLUIElement::FLUIElement( float x, float y ) {
	offset.x = x;
	offset.y = y;
}

std::vector<fl_colored_vertex>& FLUIElement::get_vertices() {
	return vertices;
}

