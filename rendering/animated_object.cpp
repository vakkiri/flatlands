/*
 * 	animated_object.cpp
 *
 */

#include "renderer.h"
#include "animated_object.h"

FLAnimatedObject::FLAnimatedObject(unsigned int num_animations) : FLTexturedObject() {
	active = true;
	repeats = true;
	animation_finished = false;
	elapsed_frames = 0;
	elapsed_start_steps = 0;
	cur_step = 0;
	this->num_animations = num_animations;
	start_repeat = std::vector<unsigned int>(num_animations, 0);

	Renderer::getInstance().add_animated_object( this );
}

FLAnimatedObject::FLAnimatedObject( unsigned int num_animations, unsigned int num_steps, unsigned int frames_per_step, float step ) : FLAnimatedObject( num_animations ) {
	this->num_steps = num_steps;
	this->frames_per_step = frames_per_step;
	this->s_step = step;
	this->t_step = step;
	cur_animation = 0;

	// TODO: should take separate parameters for s and t steps
	// will implement as soon as needed...
}

FLAnimatedObject::FLAnimatedObject( unsigned int num_animations, unsigned int num_steps, unsigned int frames_per_step, float step, bool repeats ) : 
	FLAnimatedObject( num_animations, num_steps, frames_per_step, step ) {
	this->repeats = repeats;
}

void FLAnimatedObject::update_animation() {
	if ( active ) {
		if ( elapsed_frames++ >= frames_per_step ) {
			elapsed_frames = 0;

			if ( cur_step == 0 ) {
				if ( ++elapsed_start_steps >= start_repeat[cur_animation] ) {
					++cur_step;
					elapsed_start_steps = 0;
				}
			}
			else 
				++cur_step;

			if ( cur_step > num_steps ) {
				if ( !repeats ) {
					active = false;
					animation_finished = true;
				}
				else
					cur_step = 0;
			}
		}
	}
}

void FLAnimatedObject::set_animation( unsigned int animation ) {
	cur_animation = animation;
}

void FLAnimatedObject::start_animation() { active = true; }

void FLAnimatedObject::stop_animation() { active = false; }

void FLAnimatedObject::reset_animation() {
	elapsed_frames = 0;
	elapsed_start_steps = 0;
	cur_step = 0;
}

void FLAnimatedObject::set_start_repeat( unsigned int repeat, unsigned int animation ) { start_repeat[animation] = repeat; }

float FLAnimatedObject::s() { return _s + ( s_step * cur_step ); }
float FLAnimatedObject::t() { return _t + ( t_step * cur_animation ); }

void FLAnimatedObject::set_repeats( bool repeats ) {
	this->repeats = repeats;
}

