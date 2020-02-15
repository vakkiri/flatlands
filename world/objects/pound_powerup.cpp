/*
 * 	pound_powerup.cpp
 *
 */

#include "pound_powerup.h"

#include "../world_environment.h"
#include "../player/player.h"
#include "../../rendering/renderer.h"

#define NUM_ANIMATIONS 1
#define NUM_STEPS 3
#define FRAMES_PER_STEP 5
#define STEP 16
#define REPEATS true

#define S 0
#define T 80
#define SIZE 16

FLPoundPowerup::FLPoundPowerup( float x, float y ) :
	FLGameObject( x, y, SIZE, SIZE ),
	FLAnimatedObject(
			NUM_ANIMATIONS,
			NUM_STEPS,
			FRAMES_PER_STEP,
			STEP,
			REPEATS
			) {
	set_st( S, T );

	Renderer::getInstance().add_to_world( this );
}

FLPoundPowerup::~FLPoundPowerup() {
	Renderer::getInstance().remove_from_world( this );
}

void FLPoundPowerup::collide_with( FLPlayer *player ) {
	player->set_ability( FL_GROUND_POUND );
	set_repeats( false );
}

void FLPoundPowerup::update_animation() {
	FLAnimatedObject::update_animation();

	if ( animation_finished )
		delete this;
}

