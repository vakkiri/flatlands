/*
 * 	fl_animator.cpp
 *
 */

#include <iostream>
#include "fl_animator.h"
#include "common/fl_static_buffer.h"
#include "components/texture/fl_texture.h"
#include "resources/fl_collections.h"

#define DEFAULT_NUM_ANIMATORS 2000

namespace FLAnimators {
	FLStaticBuffer<FLAnimator> animators(DEFAULT_NUM_ANIMATORS);

	void set_collection(fl_handle handle, std::string collection) {
		FLCollection& c = FLCollections::get(collection);

		animators[handle].num_frames = c.num_elements;
		animators[handle].collection = collection;

		if (animators[handle].frame >= animators[handle].num_frames) {
			animators[handle].frame = 0;
			animators[handle].tick = 0;
		}
	}

	fl_handle create(fl_handle texture, std::string collection, unsigned int num_ticks, bool repeats) {
		fl_handle handle = animators.create();

		if (handle != NULL_HANDLE) {
			animators[handle].texture = texture;
			animators[handle].paused = false;
			animators[handle].finished = false;
			animators[handle].repeats = repeats;
			animators[handle].tick = 0;
			animators[handle].frame = 0;
			animators[handle].num_ticks = num_ticks;
			set_collection(handle, collection);
		}

		return handle;
	}

	fl_handle create(fl_handle texture, std::string collection, unsigned int num_ticks) {
		return create(texture, collection, num_ticks, true);
	}

	void destroy(fl_handle handle) {
		animators.destroy(handle);
	}

	bool finished(fl_handle handle) {
		return (handle == NULL_HANDLE || animators[handle].finished);
	}

	void update() {
		for (FLAnimator& animator : animators) {
			FLTexture* tex = FLTextures::get(animator.texture);
			FLCollection& collection = FLCollections::get(animator.collection);
			FLCollectionElement element = collection.elements[animator.frame];

			if (tex != nullptr) {
				tex->s = element.s;
				tex->t = element.t;
				tex->w = element.w;
				tex->h = element.h;
			} else {
				std::cout << "Warning: animator with no texture\n";
			}

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
}
