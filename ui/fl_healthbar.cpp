/*
 * 	fl_healthbar.cpp
 */

#include <iostream>
#include "fl_healthbar.h"

#include "../world/world_environment.h"
#include "../world/player/player.h"

#define X_POS	16
#define Y_POS	16

FLHealthbar::FLHealthbar() : FLUIElement( X_POS, Y_POS ) {
	init_vertices();
	init_textures();
}

FLHealthbar::~FLHealthbar() {
	delete icon;
	delete bar_image;
}

std::vector<fl_colored_vertex>& FLHealthbar::get_primitive_vertices() {
	float ratio = FLWorldEnvironment::getInstance().player()->health_ratio();

	primitive_vertices[1].pos.x = primitive_vertices[0].pos.x + (94 * ratio);
	primitive_vertices[1].pos.y = primitive_vertices[0].pos.y;

	primitive_vertices[2].pos.x = primitive_vertices[1].pos.x;
	primitive_vertices[2].pos.y = primitive_vertices[1].pos.y + 4;

	return FLUIElement::get_primitive_vertices();
}

void FLHealthbar::init_vertices() {
	primitive_vertices = std::vector<fl_colored_vertex>(4);

	primitive_vertices[0].pos.x = offset.x + 24;
	primitive_vertices[0].pos.y = offset.y + 6;

	primitive_vertices[3].pos.x = primitive_vertices[0].pos.x;
	primitive_vertices[3].pos.y = primitive_vertices[0].pos.y + 4;

	primitive_vertices[0].c = {255.f/255.f, 194.f/255.f, 161.f/255.f, 1.f};
	primitive_vertices[1].c = primitive_vertices[0].c;
	primitive_vertices[2].c = primitive_vertices[0].c;
	primitive_vertices[3].c = primitive_vertices[0].c;
}

void FLHealthbar::init_textures() {
	icon = new FLTexturedObject( offset.x, offset.y, 16, 16 );
	icon->set_st(0, 0);
	
	textured_objects.push_back( icon );

	bar_image = new FLTexturedObject( offset.x + 22, offset.y, 96, 16 );
	bar_image->set_st(16, 0);

	textured_objects.push_back( bar_image );
}

