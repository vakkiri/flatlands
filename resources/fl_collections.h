/*
 * 	fl_collections.h
 *
 * 	Collections of images, generally for animations.
 *
 */

#ifndef FL_COLLECTIONS_H_
#define FL_COLLECTIONS_H_

#include <string>

struct FLCollectionElement {
	float s;
	float t;
	float w;
	float h;
};

struct FLCollection {
	FLCollectionElement* elements;
	unsigned int num_elements;
};

namespace FLCollections {
	void clear();

	FLCollection& get(std::string name);
	void set_element(std::string name, int index, float s, float t, float w, float h);
	void add_collection(std::string name, unsigned int num_elements);
}

#endif
