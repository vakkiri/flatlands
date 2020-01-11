/*
 * 	destroyable_tile.cpp
 *
 */

#include "destroyable_tile.h"

#include "../../rendering/renderer.h"
#include "../../rendering/world_surface.h"
#include "../../tilemap/tilemap.h"
#include "../world_environment.h"

#define NUM_ANIMATIONS 1
#define NUM_STEPS 4
#define FRAMES_PER_STEP 20
#define STEP 16
#define REPEATS false

#define S 0
#define T 64
#define SIZE 16

FLDestroyableTile::FLDestroyableTile( float x, float y ) : 
	FLWorldObject( x, y, SIZE, SIZE ), 
	FLAnimatedObject( 
			NUM_ANIMATIONS,
			NUM_STEPS,
			FRAMES_PER_STEP,
			STEP,
			REPEATS
	       	) {
		stop_animation();	// only start animation when broken
		set_st( S, T );

		Renderer::getInstance().get_world_surface()->add_object(this);
		FLWorldEnvironment::getInstance().tilemap()->set_solid_at( this->x(), this->y(), w(), h(), true );
};

