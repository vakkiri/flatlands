/*
 * 	effect.cpp
 *
 */

#include "effect.h"

#include "../rendering/renderer.h"

#define REPEATS false
#define NUM_ANIMATIONS 1
#define FRAMES_PER_STEP 1

FLEffect::FLEffect( float x, float y, float s, float t, float num_frames, float w, float h ) :
	FLGameObject( x, y, w, h ),
	FLAnimatedObject(
			NUM_ANIMATIONS,
			num_frames,
			FRAMES_PER_STEP,
			w,
			REPEATS
			) {
	set_st( s, t );

	Renderer::getInstance().add_to_world( this );
}

FLEffect::~FLEffect() {
	Renderer::getInstance().remove_from_world( this );
}

void FLEffect::update_animation() {
	FLAnimatedObject::update_animation();

	if ( animation_finished )
		delete this;
}

			
