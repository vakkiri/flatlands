/*
 * 	fl_dialogue_box.cpp
 *
 */

#include <iostream>

#include "../rendering/renderer.h"
#include "../rendering/fl_text_surface.h"
#include "fl_dialogue_box.h"
#include "fl_ui_manager.h"

FLDialogueBox::FLDialogueBox() : FLDialogueBox("", 0, 0) {}

FLDialogueBox::FLDialogueBox(std::string text, float x, float y)
	: FLUIElement(x, y) {
	// TODO: process text into single-box sized messages
	messages.push_back(text);
	// TODO: set appropriate text widths
	width = 128;
	height = 64;
	border_size = 8;

	init_vertices();

	FLUIManager::getInstance().add_element(this);
	FLUIManager::getInstance().set_active_element(this);
}

void FLDialogueBox::accept() {
	messages.pop_back();

	if (messages.empty()) {
		FLUIManager::getInstance().remove_element(this);
		delete this;
	} else {
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
	primitive_vertices.push_back(std::vector<fl_colored_vertex>());
	fl_color c0 = {20.f/255.f, 24.f/255.f, 46.f/255.f, 1.f};

	point p0 = {offset.x + border_size, offset.y + height};
	point p1 = {offset.x, offset.y + height - border_size};
	point p2 = {offset.x, offset.y + border_size};
	point p3 = {offset.x + border_size, offset.y};
	point p4 = {offset.x + width - border_size, offset.y};
	point p5 = {offset.x + width, offset.y + border_size};
	point p6 = {offset.x + width, offset.y + height - border_size};
	point p7 = {offset.x + width - border_size, offset.y + height};

	fl_colored_vertex v0 = {p0, c0};
	fl_colored_vertex v1 = {p1, c0};
	fl_colored_vertex v2 = {p2, c0};
	fl_colored_vertex v3 = {p3, c0};
	fl_colored_vertex v4 = {p4, c0};
	fl_colored_vertex v5 = {p5, c0};
	fl_colored_vertex v6 = {p6, c0};
	fl_colored_vertex v7 = {p7, c0};

	primitive_vertices[0].push_back(v0);
	primitive_vertices[0].push_back(v1);
	primitive_vertices[0].push_back(v2);
	primitive_vertices[0].push_back(v3);
	primitive_vertices[0].push_back(v4);
	primitive_vertices[0].push_back(v5);
	primitive_vertices[0].push_back(v6);
	primitive_vertices[0].push_back(v7);

}

std::string FLDialogueBox::get_message() {
	return messages.front();
}

// TODO: this should be replaced with a render() function instead of returning verts wtf lmao
// i see why i thought this may have been a good idea
// but it was quite a bad idea

std::vector<std::vector<fl_colored_vertex>>& FLDialogueBox::get_primitive_vertices() {
	FLTextSurface* text_surface = Renderer::getInstance().get_text_surface();
	std::string test = "sorrow, very";
	float x = offset.x + border_size;
	float y = offset.y + 4;
	for (auto c : test) {
		text_surface->add_character(x + 3, y, c);
		x += 6;
	}
	return FLUIElement::get_primitive_vertices();
}
