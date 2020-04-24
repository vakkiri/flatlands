/*
 * 	animated_object.cpp
 *
 */

#include <algorithm>
#include "renderer.h"
#include "animated_object.h"

std::vector<FLAnimatedObject*> animated_objects;

FLAnimatedObject::FLAnimatedObject(unsigned int num_animations) : FLTexturedObject() {
	active = true;
	repeats = true;
	animation_finished = false;
	elapsed_frames = 0;
	elapsed_start_steps = 0;
	cur_step = 0;
	this->num_animations = num_animations;
	start_repeat = std::vector<unsigned int>(num_animations, 0);

	animated_object_list_position = animated_objects.size();
	animated_objects.push_back( this );
}

FLAnimatedObject::FLAnimatedObject( unsigned int num_animations, unsigned int num_steps, unsigned int frames_per_step, float sstep, float tstep ) : FLAnimatedObject( num_animations ) {
	this->num_steps = num_steps;
	this->frames_per_step = frames_per_step;
	this->s_step = sstep;
	this->t_step = tstep;
	cur_animation = 0;
}

FLAnimatedObject::FLAnimatedObject( unsigned int num_animations, unsigned int num_steps, unsigned int frames_per_step, float step ) : FLAnimatedObject( num_animations, num_steps, frames_per_step, step, step) {}

FLAnimatedObject::FLAnimatedObject( unsigned int num_animations, unsigned int num_steps, unsigned int frames_per_step, float step, bool repeats ) : 
	FLAnimatedObject( num_animations, num_steps, frames_per_step, step ) {
	this->repeats = repeats;
}

FLAnimatedObject::~FLAnimatedObject() {
	animated_objects[animated_object_list_position] = nullptr;
}

void FLAnimatedObject::update_animation() {
	if ( active ) {
		if ( elapsed_frames++ >= frames_per_step ) {
			elapsed_frames = 0;

			if ( ++cur_step >= num_steps ) {
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

void FLAnimatedObject::set_animated_object_list_position( unsigned int position ) {
	animated_object_list_position = position;
}

std::vector<FLAnimatedObject*>& get_animated_objects() {
	return animated_objects;
}

void remove_null_animated_objects() {
	animated_objects.erase(
			std::remove_if(
				animated_objects.begin(),
				animated_objects.end(),
				[](FLAnimatedObject* obj) { return obj == nullptr; } ),
			animated_objects.end() );

	for ( int i = 0; i < animated_objects.size(); ++i )
		animated_objects[i]->set_animated_object_list_position( i );
}

void clear_animated_objects() {
	animated_objects.clear();
}


