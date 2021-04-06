/*
 * 	fl_animator.cpp
 *
 */

#include <iostream>
#include "fl_animator.h"
#include "common/fl_static_buffer.h"

#define DEFAULT_NUM_ANIMATORS 2000

namespace FLAnimators {
	FLStaticBuffer<FLAnimator> animators(DEFAULT_NUM_ANIMATORS);

	fl_handle create(std::string animation_name) {
		(void) animation_name;
		fl_handle handle = animators.create();

		if (handle != NULL_HANDLE) {
			// initialize...
		}

		return handle;
	}

	void destroy(fl_handle handle) {
		animators.destroy(handle);
	}

	void update() {
		for (auto animator : animators) {
			if (!animator.paused) {
				if (++animator.tick >= animator.num_ticks) {
					animator.tick = 0;
					if (++animator.frame >= animator.num_frames) {
						if (animator.repeats) {
							animator.frame = 0;
						} else {
							animator.finished = true;
							animator.paused = true;
						}
					}
				}
			}
		}
	}

	float s(fl_handle handle) {
		if (handle != NULL_HANDLE) {
			FLAnimator* animator = &animators[handle];
			return animator->s[animator->frame];
		} else {
			std::cout << "Warning: Tried to access animator with null handle.\n";
			return 0;
		}
	}

	float t(fl_handle handle) {
		if (handle != NULL_HANDLE) {
			FLAnimator* animator = &animators[handle];
			return animator->t[animator->frame];
		} else {
			std::cout << "Warning: Tried to access animator with null handle.\n";
			return 0;
		}
	}

}
