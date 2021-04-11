/*
 * 	fl_animator.h
 *
 * 	Defines animations sequences for sprites.
 *
 */

#ifndef FL_ANIMATOR_H_
#define FL_ANIMATOR_H_

#include <functional>
#include <string>
#include <vector>

#include "common/basic_types.h"

#define NO_ANIMATOR NULL_HANDLE

struct FLAnimator {
	std::string collection;
	// TODO: maybe map<int, function> instead idk
	//std::vector<std::function<void()>> callbacks[MAX_STEPS];
	unsigned int tick;
	unsigned int num_ticks;
	unsigned int frame;
	unsigned int num_frames;
	bool paused;
	bool finished;
	bool repeats;
	fl_handle texture;
};

namespace FLAnimators {
	void update();

	fl_handle create(fl_handle texture, std::string collection, unsigned int num_ticks);
	fl_handle create(fl_handle texture, std::string collection, unsigned int num_ticks, bool repeats);
	void set_collection(fl_handle handle, std::string collection);
	void destroy(fl_handle handle);

	bool finished(fl_handle handle);
}

#endif

