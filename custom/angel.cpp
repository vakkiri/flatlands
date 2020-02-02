/*
 * 	angel.cpp
 *
 */

#include "angel.h"

#include "../rendering/renderer.h"

#define NUM_ANIMATIONS 1
#define FRAMES_PER_STEP 1
#define REPEATS true

#define S (0)
#define T (256 - 64)
#define W (160)
#define H (96)

#define STEP 0
#define NUM_STEPS 1

NVAngel::NVAngel( float x, float y ) :
	FLGameObject( x - 48.f, y, W, H ),
	FLAnimatedObject( 
			NUM_ANIMATIONS,
			NUM_STEPS,
			FRAMES_PER_STEP,
			STEP,
			REPEATS) {
	set_st( S, T );

	Renderer::getInstance().add_to_world( this );
}

NVAngel::~NVAngel() {
	Renderer::getInstance().remove_animated_object( this );
	Renderer::getInstance().remove_from_world( this );
}


