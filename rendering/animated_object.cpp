/*
 * 	animated_object.cpp
 *
 */

#include <iostream>

#include "animated_object.h"

FLAnimatedObject::FLAnimatedObject() : FLTexturedObject() {
	active = true;
	repeats = true;
	elapsed_frames = 0;
	cur_step = 0;
}

FLAnimatedObject::FLAnimatedObject( unsigned int num_steps, unsigned int frames_per_step, float step ) : FLAnimatedObject() {
	this->num_steps = num_steps;
	this->frames_per_step = frames_per_step;
	this->s_step = step;
	this->t_step = step;
	cur_animation = 0;
	cur_step = 0;
	// TODO: should take separate parameters for s and t steps
}

FLAnimatedObject::FLAnimatedObject( unsigned int num_steps, unsigned int frames_per_step, float step, bool repeats ) : FLAnimatedObject( num_steps, frames_per_step, step ) {
	this->repeats = repeats;
}

void FLAnimatedObject::start_animation() { active = true; }
void FLAnimatedObject::stop_animation() { active = false; }

void FLAnimatedObject::update_animation() {
	if ( active ) {
		if ( elapsed_frames++ >= frames_per_step ) {
			elapsed_frames = 0;

			if ( cur_step++ >= num_steps ) {
				cur_step = 0;

			if ( !repeats )
					active = false;
			}
		}
	}
}

float FLAnimatedObject::s() { return _s + ( s_step * cur_step ); }
float FLAnimatedObject::t() { return _t + ( t_step * cur_animation ); }

