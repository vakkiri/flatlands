/*
 * 	toggle_tile.cpp
 *
 */

#include <iostream>
#include <algorithm>

#include "toggle_tile.h"

#include "../../rendering/renderer.h"
#include "../../tilemap/tilemap.h"
#include "../world_environment.h"
#include "../player/player.h"

#define NUM_ANIMATIONS 1
#define NUM_STEPS 2
#define FRAMES_PER_STEP 5
#define STEP 16
#define REPEATS true

#define S 0
#define T 288
#define SIZE 16

// module members
std::vector<FLToggleTile*> toggle_tiles;

FLToggleTile::FLToggleTile( float x, float y, bool on ) : 
	FLGameObject( x, y, SIZE, SIZE ),
	FLAnimatedObject( 
			NUM_ANIMATIONS,
			NUM_STEPS,
			FRAMES_PER_STEP,
			STEP,
			REPEATS
	       	) {
	FLWorldEnvironment& environment = FLWorldEnvironment::getInstance();
	set_st( S, T );

	Renderer::getInstance().add_to_world( this );

	environment.tilemap()->set_solid_at( this->x(), this->y(), SIZE, SIZE, on );
	this->on = on;
	toggle_tiles.push_back(this);
};

FLToggleTile::~FLToggleTile() {
	Renderer::getInstance().remove_from_world( this );
}

void FLToggleTile::toggle() {
	on = !on;
	FLWorldEnvironment::getInstance().tilemap()->set_solid_at( this->x(), this->y(), SIZE, SIZE, on );
}

void FLToggleTile::update_animation() {
	if (on)
		cur_step = 0;
	else
		cur_step = 1;
}

std::vector<FLToggleTile*>& get_toggle_tiles() {
	return toggle_tiles;
}

void clear_toggle_tiles() {
	for ( auto tile : toggle_tiles ) {
		if ( tile != nullptr )
			delete tile;
	}

	toggle_tiles.clear();
}

