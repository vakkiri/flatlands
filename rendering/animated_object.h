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

class FLAnimatedObject : public FLTexturedObject {
	public:
		FLAnimatedObject();
		FLAnimatedObject(unsigned int num_steps, unsigned int frames_per_step, float step);
		FLAnimatedObject(unsigned int num_steps, unsigned int frames_per_step, float step, bool repeats);

		virtual void update_animation();
		void start_animation();
		void stop_animation();

		virtual float s();

	protected:
		float step;	// how much to move _s by each frame
		unsigned int cur_step;
		unsigned int num_steps;
		unsigned int frames_per_step;
		unsigned int elapsed_frames;
		bool repeats;
		bool active;
};

#endif

