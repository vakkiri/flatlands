/*
 * 	fl_healthbar.cpp
 */

#include "fl_healthbar.h"
#include <iostream>

#include "../environment/fl_environment.h"
#include "../game/fl_game.h"
#include "../rendering/textured_object.h"
#include "../world/player/player.h"

#define X_POS 16
#define Y_POS 16

FLHealthbar::FLHealthbar() : FLUIElement(X_POS, Y_POS) {
	init_vertices();
	init_textures();
}

std::vector<std::vector<fl_colored_vertex>> &
FLHealthbar::get_primitive_vertices() {
	float ratio = FLGame::instance().environment()->player()->health_ratio();

	primitive_vertices[0][1].pos.x =
		primitive_vertices[0][0].pos.x + (94 * ratio);
	primitive_vertices[0][1].pos.y = primitive_vertices[0][0].pos.y;

	primitive_vertices[0][2].pos.x = primitive_vertices[0][1].pos.x;
	primitive_vertices[0][2].pos.y = primitive_vertices[0][1].pos.y + 4;

	primitive_vertices[1][1].pos.x =
		primitive_vertices[0][0].pos.x + 94.f * ratio * 0.9;
	primitive_vertices[1][1].pos.y = primitive_vertices[0][1].pos.y;
	primitive_vertices[1][2].pos.x =
		primitive_vertices[0][0].pos.x + 94.f * ratio * 0.8;
	primitive_vertices[1][2].pos.y = primitive_vertices[0][2].pos.y;

	primitive_vertices[2][1].pos.x =
		primitive_vertices[0][0].pos.x + 94.f * ratio * 0.5;
	primitive_vertices[2][1].pos.y = primitive_vertices[1][1].pos.y;
	primitive_vertices[2][2].pos.x =
		primitive_vertices[0][0].pos.x + 94.f * ratio * 0.4;
	primitive_vertices[2][2].pos.y = primitive_vertices[1][2].pos.y;

	primitive_vertices[3][1].pos.x =
		primitive_vertices[0][0].pos.x + 94.f * ratio * 0.25;
	primitive_vertices[3][1].pos.y = primitive_vertices[2][1].pos.y;
	primitive_vertices[3][2].pos.x =
		primitive_vertices[0][0].pos.x + 94.f * ratio * 0.15;
	primitive_vertices[3][2].pos.y = primitive_vertices[2][2].pos.y;

	return FLUIElement::get_primitive_vertices();
}

void FLHealthbar::init_vertices() {
	primitive_vertices = std::vector<std::vector<fl_colored_vertex>>(
		4, std::vector<fl_colored_vertex>(4));

	for (int i = 0; i < 4; ++i) {
		primitive_vertices[i][0].pos.x = offset.x + 24;
		primitive_vertices[i][0].pos.y = offset.y + 6;

		primitive_vertices[i][3].pos.x = primitive_vertices[i][0].pos.x;
		primitive_vertices[i][3].pos.y = primitive_vertices[i][0].pos.y + 4;
	}

	for (int i = 0; i < 4; ++i) {
		primitive_vertices[0][i].c = {255.f / 255.f, 194.f / 255.f,
									  161.f / 255.f, 1.f};
	}

	for (int i = 0; i < 4; ++i) {
		primitive_vertices[1][i].c = {230.f / 255.f, 69.f / 255.f, 57.f / 255.f,
									  1.f};
	}

	for (int i = 0; i < 4; ++i) {
		primitive_vertices[2][i].c = {156.f / 255.f, 42.f / 255.f,
									  112.f / 255.f, 1.f};
	}

	for (int i = 0; i < 4; ++i) {
		primitive_vertices[3][i].c = {105.f / 255.f, 36.f / 255.f,
									  100.f / 255.f, 1.f};
	}
}

void FLHealthbar::init_textures() {
	icon = new FLTexturedObject(offset.x, offset.y, 16, 16);
	icon->set_st(0, 0);

	textured_objects.push_back(icon);

	bar_image = new FLTexturedObject(offset.x + 22, offset.y, 96, 16);
	bar_image->set_st(16, 0);

	textured_objects.push_back(bar_image);
}
