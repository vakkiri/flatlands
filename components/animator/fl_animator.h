/*
 * 	fl_animator.h
 *
 * 	Defines animations sequences for sprites.
 *
 */

#ifndef FL_ANIMATOR_H_
#define FL_ANIMATOR_H_

#include "common/basic_types.h"

#define NO_ANIMATOR NULL_HANDLE
#define MAX_STEPS 16

struct FLAnimator {
	float s[MAX_STEPS];
	float t[MAX_STEPS];
	unsigned int tick;
	unsigned int num_ticks;
	unsigned int frame;
	unsigned int num_frames;
	bool paused;
	bool repeats;
};

namespace FLAnimators {
	void update();
	void destroy(fl_handle handle);

	float s(fl_handle handle);
	float t(fl_handle handle);

	fl_handle create(std::string animation_name);
}

#endif

