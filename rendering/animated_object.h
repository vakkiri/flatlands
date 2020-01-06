/*
 * 	animated_object.h
 *
 * 	Base abstract class for any object which is animated via
 * 	multiple distinct images.
 *
 */

#ifndef ANIMATED_OBJECT_H_
#define ANIMATED_OBJECT_H_

#include "textured_object.h"

class FLAnimatedObject : virtual public FLTexturedObject {
	public:
		FLAnimatedObject();
		FLAnimatedObject(unsigned int num_steps, unsigned int frames_per_step, float step);
		FLAnimatedObject(unsigned int num_steps, unsigned int frames_per_step, float step, bool repeats);

		virtual void update_animation();
		void start_animation();
		void stop_animation();

		virtual float s();
		virtual float t();

	protected:
		float s_step;	// how much to move _s by each frame
		float t_step;	// how much to move _t by when animation changes
		unsigned int cur_step;
		unsigned int num_steps;
		unsigned int frames_per_step;
		unsigned int elapsed_frames;
		unsigned int cur_animation;
		bool repeats;
		bool active;
};

#endif

