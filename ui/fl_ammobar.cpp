/*
 * 	fl_ammobar.cpp
 */

#include <iostream>
#include "fl_ammobar.h"

#include "../environment/fl_environment.h"
#include "../game/fl_game.h"
#include "../world/player/player.h"
#include "../rendering/textured_object.h"

#define X_POS	16
#define Y_POS	34

FLAmmobar::FLAmmobar() : FLUIElement( X_POS, Y_POS ) {
	init_vertices();
	init_textures();
}

FLAmmobar::~FLAmmobar() {
	delete icon;
	delete bar_image;
}

std::vector<fl_colored_vertex>& FLAmmobar::get_primitive_vertices() {
	float ratio = FLGame::instance().environment()->player()->clip_ratio();

	primitive_vertices[1].pos.x = primitive_vertices[0].pos.x + (59 * ratio);
	primitive_vertices[1].pos.y = primitive_vertices[0].pos.y;

	primitive_vertices[2].pos.x = primitive_vertices[1].pos.x;
	primitive_vertices[2].pos.y = primitive_vertices[1].pos.y + 3;

	return FLUIElement::get_primitive_vertices();
}

void FLAmmobar::init_vertices() {
	primitive_vertices = std::vector<fl_colored_vertex>(4);

	primitive_vertices[0].pos.x = offset.x + 25;
	primitive_vertices[0].pos.y = offset.y + 6;

	primitive_vertices[3].pos.x = primitive_vertices[0].pos.x;
	primitive_vertices[3].pos.y = primitive_vertices[0].pos.y + 3;

	primitive_vertices[0].c = {79.f/255.f, 164.f/255.f, 184.f/255.f, 1.f};
	primitive_vertices[1].c = primitive_vertices[0].c;
	primitive_vertices[2].c = primitive_vertices[0].c;
	primitive_vertices[3].c = primitive_vertices[0].c;
}

void FLAmmobar::init_textures() {
	icon = new FLTexturedObject( offset.x, offset.y, 16, 16 );
	icon->set_st(0, 16);
	
	textured_objects.push_back( icon );

	bar_image = new FLTexturedObject( offset.x + 22, offset.y, 64, 16 );
	bar_image->set_st(16, 16);

	textured_objects.push_back( bar_image );
}

