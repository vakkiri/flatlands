/*
 * 	effect.cpp
 *
 */

#include "effect.h"

#include "../rendering/renderer.h"

FLEffect::FLEffect(FLTexturedObjectParams tex_params,
				   FLAnimatedObjectParams anim_params, float s, float t)
	: FLAnimatedObject(tex_params, anim_params) {
	set_st(s, t);

	Renderer::getInstance().add_to_world(this);
}

FLEffect::~FLEffect() { Renderer::getInstance().remove_from_world(this); }

void FLEffect::update_animation() {
	FLAnimatedObject::update_animation();

	if (animation_finished)
		delete this;
}
