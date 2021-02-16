/*
 * 	fl_ammobar.cpp
 */

#include "fl_ammobar.h"
#include <iostream>

#include "../environment/fl_environment.h"
#include "../game/fl_game.h"
#include "../rendering/fl_text_surface.h"
#include "../rendering/textured_object.h"
#include "../rendering/renderer.h"
#include "../world/player/player.h"

#define X_POS 16
#define Y_POS 34

FLAmmobar::FLAmmobar() : FLUIElement(X_POS, Y_POS) {
	init_vertices();
	init_textures();
}

std::vector<std::vector<fl_colored_vertex>> &
FLAmmobar::get_primitive_vertices() {
	// this does not belong here lol refactor plz eeee
	// --
	Renderer& r = Renderer::getInstance();
	FLTextSurface* text_surface = r.get_text_surface();
	std::string ammo = std::to_string(FLGame::instance().environment()->player()->get_ammo());

	for (unsigned int i = 0; i < ammo.size(); ++i) {
		text_surface->add_character(offset.x + 90 + 5 * i, offset.y + 4, ammo[i]);
	}
	// --

	float ratio = FLGame::instance().environment()->player()->clip_ratio();

	primitive_vertices[0][1].pos.x =
		primitive_vertices[0][0].pos.x + (59 * ratio);
	primitive_vertices[0][1].pos.y = primitive_vertices[0][0].pos.y;

	primitive_vertices[0][2].pos.x = primitive_vertices[0][1].pos.x;
	primitive_vertices[0][2].pos.y = primitive_vertices[0][1].pos.y + 3;

	return FLUIElement::get_primitive_vertices();
}

void FLAmmobar::init_vertices() {
	primitive_vertices = std::vector<std::vector<fl_colored_vertex>>(
		1, std::vector<fl_colored_vertex>(4));

	primitive_vertices[0][0].pos.x = offset.x + 25;
	primitive_vertices[0][0].pos.y = offset.y + 6;

	primitive_vertices[0][3].pos.x = primitive_vertices[0][0].pos.x;
	primitive_vertices[0][3].pos.y = primitive_vertices[0][0].pos.y + 3;

	primitive_vertices[0][0].c = {79.f / 255.f, 164.f / 255.f, 184.f / 255.f,
								  1.f};
	primitive_vertices[0][1].c = primitive_vertices[0][0].c;
	primitive_vertices[0][2].c = primitive_vertices[0][0].c;
	primitive_vertices[0][3].c = primitive_vertices[0][0].c;
}

void FLAmmobar::init_textures() {
	icon_id = new_texturer();
	bar_image_id = new_texturer();

	if (icon_id > 0) {
		FLTexturedObject* icon = get_texturer(icon_id);
		icon->set_x(offset.x);
		icon->set_y(offset.y);
		icon->set_w(16);
		icon->set_h(16);
		icon->set_st(0, 16);
		textured_objects.push_back(icon_id);
	}

	if (bar_image_id > 0) {
		FLTexturedObject* bar_image = get_texturer(bar_image_id);
		bar_image->set_x(offset.x + 22);
		bar_image->set_y(offset.y);
		bar_image->set_w(64);
		bar_image->set_h(16);
		bar_image->set_st(16, 16);
		textured_objects.push_back(bar_image_id);
	}
}
