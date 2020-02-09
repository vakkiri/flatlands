/*
 * 	fl_ui_element.cpp
 *
 */

#include "fl_ui_element.h"

FLUIElement::FLUIElement() : FLUIElement(0, 0) {}

FLUIElement::FLUIElement( float x, float y ) {
	offset.x = x;
	offset.y = y;
}

