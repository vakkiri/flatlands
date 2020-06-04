/*
 * 	animated_object.cpp
 *
 */

#include <algorithm>
#include "renderer.h"
#include "animated_object.h"

std::vector<FLAnimatedObject*> animated_objects;

FLAnimatedObject::FLAnimatedObject( FLTexturedObjectParams& tex_params, FLAnimatedObjectParams& anim_params ) :
	FLTexturedObject( tex_params ) {
	active = true;
	animation_finished = false;
	elapsed_frames = 0;
	elapsed_start_steps = 0;
	cur_step = 0;
	cur_animation = 0;
	has_update_method = false;

	this->num_steps = anim_params.num_steps;
	this->frames_per_step = anim_params.frames_per_step;
	this->s_step = anim_params.sstep;
	this->t_step = anim_params.tstep;
	this->repeats = anim_params.repeats;
	this->num_animations = anim_params.num_animations;

	start_repeat = std::vector<unsigned int>(num_animations, 0);
	animated_object_list_position = animated_objects.size();
	animated_objects.push_back( this );
}

FLAnimatedObject::~FLAnimatedObject() {
	animated_objects[animated_object_list_position] = nullptr;
}

void FLAnimatedObject::update_animation() {
	if ( has_update_method ) {
		on_animation_update();
	}
	if ( active ) {
		if (elapsed_frames == 0 && cur_step == 0) {
			run_start_callbacks();
		}

		if ( elapsed_frames++ >= frames_per_step ) {
			elapsed_frames = 0;

			if ( ++cur_step >= num_steps ) {
				if ( !repeats ) {
					active = false;
					animation_finished = true;
				}
				else {
					cur_step = 0;
				}
			}
		}
	}
}

void FLAnimatedObject::set_animation( unsigned int animation ) {
	cur_animation = animation;
}

void FLAnimatedObject::start_animation() { 
	active = true; 
	animation_finished = false;
}

void FLAnimatedObject::stop_animation() { active = false; }

void FLAnimatedObject::reset_animation() {
	elapsed_frames = 0;
	elapsed_start_steps = 0;
	cur_step = 0;
}

void FLAnimatedObject::set_start_repeat( unsigned int repeat, unsigned int animation ) { start_repeat[animation] = repeat; }

unsigned int FLAnimatedObject::get_animation() { return cur_animation; }
float FLAnimatedObject::s() { return _s + ( s_step * cur_step ); }
float FLAnimatedObject::t() { return _t + ( t_step * cur_animation ); }

void FLAnimatedObject::set_repeats( bool repeats ) {
	this->repeats = repeats;
}

void FLAnimatedObject::set_animated_object_list_position( unsigned int position ) {
	animated_object_list_position = position;
}

void FLAnimatedObject::set_steps( float sstep, float tstep ) {
	s_step = sstep;
	t_step = tstep;
}

bool FLAnimatedObject::finished() {
	return animation_finished;
}

void FLAnimatedObject::add_start_callback(std::function<void()> func) {
	anim_start_callbacks.push_back(func);
}

void FLAnimatedObject::run_start_callbacks() {
	for (auto f : anim_start_callbacks) {
		f();
	}
}

int FLAnimatedObject::animation() {
	return cur_animation;
}

void FLAnimatedObject::set_animation_update_method(std::function<void(void)> meth) {
	on_animation_update = meth;
	has_update_method = true;
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

	for ( unsigned int i = 0; i < animated_objects.size(); ++i )
		animated_objects[i]->set_animated_object_list_position( i );
}

void clear_animated_objects() {
	animated_objects.clear();
}

