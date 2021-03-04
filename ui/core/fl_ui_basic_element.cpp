/*
 * 	fl_ui_basic_element.cpp
 *
 */

#include "fl_ui_basic_element.h"

FLUIBasicElement::FLUIBasicElement() {
	left_element = this;
	right_element = this;
	below_element = this;
	above_element = this;
}

FLUIBasicElement::~FLUIBasicElement() {
}

FLUIBasicElement* FLUIBasicElement::up() {
	return above_element;
}

FLUIBasicElement* FLUIBasicElement::down() {
	return below_element;
}

FLUIBasicElement* FLUIBasicElement::left() {
	return left_element;
}

FLUIBasicElement* FLUIBasicElement::right() {
	return right_element;
}

void FLUIBasicElement::accept() {}

void FLUIBasicElement::render() {}

void FLUIBasicElement::update() {}
