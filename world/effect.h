/*
 * 	effect.h
 *
 * 	In-game animated effects which automatically disappear after
 * 	some time. Ie. smoke etc.
 *
 */

#ifndef EFFECT_H_
#define EFFECT_H_

#include "../common/common.h"
#include "../rendering/animated_object.h"

class FLEffect : virtual public FLAnimatedObject {
  public:
	FLEffect(FLTexturedObjectParams tex_params,
			 FLAnimatedObjectParams anim_params, float s, float t);
	virtual ~FLEffect();
	void update_animation();
};

#endif
