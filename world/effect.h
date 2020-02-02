/*
 * 	effect.h
 *
 * 	In-game animated effects which automatically disappear after
 * 	some time. Ie. smoke etc.
 *
 */

#ifndef EFFECT_H_
#define EFFECT_H_

#include "world_object.h"
#include "../rendering/animated_object.h"

class FLEffect : virtual public FLAnimatedObject, virtual public FLWorldObject {
	public:
		FLEffect( float x, float y, float s, float t, float num_frames, float w, float h );
		virtual ~FLEffect();
		void update_animation();
};

#endif

