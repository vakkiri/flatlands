/*
 * 	animated_object.h
 *
 * 	Base abstract class for any object which is animated via
 * 	multiple distinct images.
 *
 */

#ifndef ANIMATED_OBJECT_H_
#define ANIMATED_OBJECT_H_

#include <vector>
#include "textured_object.h"

class FLAnimatedObject : virtual public FLTexturedObject {
	public:
		FLAnimatedObject(unsigned int num_animations);
		FLAnimatedObject(unsigned int num_animations, unsigned int num_steps, unsigned int frames_per_step, float step);
		FLAnimatedObject(unsigned int num_animations, unsigned int num_steps, unsigned int frames_per_step, float step, bool repeats);

		virtual ~FLAnimatedObject();

		virtual void update_animation();
		void set_repeats( bool repeats );
		void start_animation();
		void reset_animation();
		void set_animation( unsigned int animation );
		void stop_animation();
		void set_start_repeat( unsigned int repeat, unsigned int animation );

		virtual float s();
		virtual float t();

		void set_animated_object_list_position( unsigned int position );

	protected:
		float s_step;	// how much to move _s by each frame
		float t_step;	// how much to move _t by when animation changes

		unsigned int cur_step;
		unsigned int num_steps;
		unsigned int frames_per_step;
		unsigned int elapsed_frames;
		unsigned int elapsed_start_steps;

		unsigned int cur_animation;
		unsigned int num_animations;

		std::vector<unsigned int> start_repeat;	//how many times to repeat the first frame of the animation
		bool repeats;
		bool active;
		bool animation_finished;

		unsigned int animated_object_list_position;
};

std::vector<FLAnimatedObject*>& get_animated_objects();
void remove_null_animated_objects();
void clear_animated_objects();

#endif

