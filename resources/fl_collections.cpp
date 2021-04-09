/*
 * 	fl_collections.cpp
 *
 */

#include <iostream>
#include <unordered_map>

#include "fl_collections.h"

namespace FLCollections {
	std::unordered_map<std::string, FLCollection> collections;

	void clear() {
		for (auto [name, collection] : collections) {
			delete [] collection.elements;
		}

		collections.clear();
	}

	FLCollection& get(std::string name) {
		return collections[name];
	}

        void set_element(std::string name, int index, float s, float t, float w, float h) {
		if (collections.find(name) != collections.end()) {
			collections[name].elements[index].s = s;
			collections[name].elements[index].t = t;
			collections[name].elements[index].w = w;
			collections[name].elements[index].h = h;
		} else {
			std::cout << "Warning: Collection " << name << " does not exist\n";
		}
	}

        void add_collection(std::string name, unsigned int num_elements) {
		if (collections.find(name) != collections.end()) {
			std::cout << "Warning: collection " << name << " already exists\n";
		} else {
			collections[name] = FLCollection {
				new FLCollectionElement[num_elements],
				num_elements
			};
		}
	}
}

