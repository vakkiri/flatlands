/*
 * 	ammo.cpp
 *
 */

#include <iostream>
#include "../../net/fl_net.h"
#include "../../rendering/renderer.h"
#include "../player/player.h"
#include "ammo.h"

#define SIZE 8

#define NUM_ANIMATIONS 1
#define NUM_STEPS 4
#define FRAMES_PER_STEP 5
#define STEP 8
#define REPEATS true

FLAmmo::FLAmmo( float x, float y, int weapon_index ) :
	FLGameObject( x, y, SIZE, SIZE ),
	FLNetObject(),
	FLAnimatedObject(
			NUM_ANIMATIONS,
			NUM_STEPS,
			FRAMES_PER_STEP,
			STEP,
			REPEATS
			) {
	
	Renderer::getInstance().add_to_world( this );
	this->weapon_index = weapon_index;

	// weapon index 0 has no ammo since it is FL_NO_WEAPON
	set_st( 96, 16 + (8 * (weapon_index - 1)) );

	if ( weapon_index == 0 )
		amt = 0;
	else if ( weapon_index == FL_FUSION )
		amt = 10;
}

FLAmmo::~FLAmmo() {
	Renderer::getInstance().remove_from_world( this );
	// TODO: call player->add_ammo
	std::cout << "Ammo deleted\n";
}

void FLAmmo::collide_with( FLPlayer *player ) {
	player->add_ammo( weapon_index, amt );
	delete this;
}

