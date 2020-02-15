/*
 * 	toggler.cpp
 *
 */

#include <iostream>
#include "toggler.h"
#include "toggle_tile.h"

#include "../../rendering/renderer.h"
#include "../../tilemap/tilemap.h"
#include "../world_environment.h"
#include "../player/player.h"

#define NUM_ANIMATIONS 1
#define NUM_STEPS 2
#define FRAMES_PER_STEP 5
#define STEP 16
#define REPEATS false
#define BOUNDS_PADDING 6.f

#define S 32
#define T 288
#define SIZE 16

#define BOUNCE_AMOUNT 7.75
#define PUSH_AMOUNT 0.5

#define COOLDOWN 30

#define MAX_OFFSET 4

FLToggler::FLToggler( float x, float y ) : 
	FLGameObject( x, y, SIZE, SIZE ),
	FLAnimatedObject( 
			NUM_ANIMATIONS,
			NUM_STEPS,
			FRAMES_PER_STEP,
			STEP,
			REPEATS
	       	) {
	FLWorldEnvironment& environment = FLWorldEnvironment::getInstance();
	stop_animation();	// only start animation when broken
	set_st( S, T );

	cooldown = 0;

	Renderer::getInstance().add_to_world( this );
	environment.tilemap()->set_solid_at( this->x(), this->y(), SIZE, SIZE, true );
};

FLToggler::~FLToggler() {
	Renderer::getInstance().remove_animated_object( this );
	Renderer::getInstance().remove_from_world( this );
}

void FLToggler::collide_with( FLPlayer *player ) {
	if ( player->pounding() && cooldown == 0) {
		std::vector<FLToggleTile*> tiles = get_toggle_tiles();

		for (FLToggleTile* tile : tiles)
			tile->toggle();

		if ( player->facing_right() )
			player->accelerate( point( PUSH_AMOUNT, -BOUNCE_AMOUNT ) );
		else
			player->accelerate( point( -PUSH_AMOUNT, -BOUNCE_AMOUNT ) );
		player->enable_ability();

		cooldown = COOLDOWN;
	}
}

float FLToggler::bounds_h() {
	return SIZE + BOUNDS_PADDING;
}

float FLToggler::bounds_y() {
	return y() - (BOUNDS_PADDING / 2.f);
}

void FLToggler::update_animation() {
	FLAnimatedObject::update_animation();

	if (cooldown > 0)
		cooldown -= 1;
}

float FLToggler::y() {
	return FLGameObject::y() + (MAX_OFFSET * cooldown/COOLDOWN);
}

