/*
 * 	fl_dialogue_box.cpp
 *
 */

#include <iostream>

#include "fl_dialogue_box.h"
#include "fl_ui_manager.h"

FLDialogueBox::FLDialogueBox() : FLDialogueBox("", 0, 0) {}

FLDialogueBox::FLDialogueBox(std::string text, float x, float y) : FLUIElement( x, y ) {
	// TODO: process text into single-box sized messages
	messages.push_back(text);
	// TODO: set appropriate text widths
	width = 128;
	height = 64;
	border_size = 4;

	init_vertices();

	FLUIManager::getInstance().add_element( this );
	FLUIManager::getInstance().set_active_element( this );
}

void FLDialogueBox::accept() {
	messages.pop_back();

	if (messages.empty()) {
		FLUIManager::getInstance().remove_element( this );
		delete this;
	}
	else {
	}
}

void FLDialogueBox::init_vertices() {
	/*
	 *	Box shape:                 .border_size
	 *		width	         . 
	 *	  v3------------v4   }...
	 *	v2		  v5
	 *	|		  |	height
	 *	v1		  v6
	 *	  v0------------v7
	 */

	fl_color c0 = {0.9, 0.1, 0.3, 1.f};
	fl_color c1 = {0.9, 0.1, 0.3, 1.f};
	fl_color c2 = {0.9, 0.1, 0.3, 1.f};
	fl_color c3 = {0.9, 0.1, 0.3, 1.f};
	fl_color c4 = {0.9, 0.1, 0.9, 1.f};
	fl_color c5 = {0.9, 0.1, 0.9, 1.f};
	fl_color c6 = {0.3, 0.1, 0.9, 1.f};
	fl_color c7 = {0.3, 0.1, 0.9, 1.f};

	point p0 = { offset.x + border_size, offset.y + height};
	point p1 = { offset.x, offset.y + height - border_size };
	point p2 = { offset.x, offset.y + border_size };
	point p3 = { offset.x + border_size, offset.y };
	point p4 = { offset.x + width - border_size, offset.y };
	point p5 = { offset.x + width, offset.y + border_size };
	point p6 = { offset.x + width, offset.y + height - border_size };
	point p7 = { offset.x + width - border_size, offset.y + height };

	fl_colored_vertex v0 = { p0, c0 };
	fl_colored_vertex v1 = { p1, c1 };
	fl_colored_vertex v2 = { p2, c2 };
	fl_colored_vertex v3 = { p3, c3 };
	fl_colored_vertex v4 = { p4, c4 };
	fl_colored_vertex v5 = { p5, c5 };
	fl_colored_vertex v6 = { p6, c6 };
	fl_colored_vertex v7 = { p7, c7 };

	vertices.push_back(v0);
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
	vertices.push_back(v4);
	vertices.push_back(v5);
	vertices.push_back(v6);
	vertices.push_back(v7);
}

