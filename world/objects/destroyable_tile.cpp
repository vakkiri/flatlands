/*
 * 	destroyable_tile.cpp
 *
 */

#include "destroyable_tile.h"

#include "../../rendering/renderer.h"
#include "../../rendering/world_surface.h"
#include "../../tilemap/tilemap.h"
#include "../world_environment.h"
#include "../player/player.h"

#define NUM_ANIMATIONS 1
#define NUM_STEPS 4
#define FRAMES_PER_STEP 20
#define STEP 16
#define REPEATS false

#define BOUNDS_PADDING 2.f

#define S 0
#define T 64
#define SIZE 16

#define BOUNCE_AMOUNT 3.2

FLDestroyableTile::FLDestroyableTile( float x, float y ) : 
	FLWorldObject( x, y, SIZE, SIZE ), 
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

		Renderer::getInstance().get_world_surface()->add_object(this);
		environment.tilemap()->set_solid_at( this->x(), this->y(), SIZE, SIZE, true );
		environment.add_object( this );
		environment.add_colliding_object( this );
};

void FLDestroyableTile::collide_with( FLPlayer *player ) {
	if ( player->pounding() ) {
		FLWorldEnvironment::getInstance().tilemap()->set_solid_at( this->x(), this->y(), SIZE, SIZE, false );
		FLWorldEnvironment::getInstance().remove_colliding_object( this );
		start_animation();
		player->accelerate( point( 0, -BOUNCE_AMOUNT ) );
	}
}

float FLDestroyableTile::bounds_h() {
	return SIZE + BOUNDS_PADDING;
}

float FLDestroyableTile::bounds_y() {
	return y() - (BOUNDS_PADDING / 2.f);
}

