/*
 * 	fl_static_buffer.h
 *
 *	Static memory management for game objects, components etc.
 *
 */

#ifndef FL_STATIC_BUFFER_H_
#define FL_STATIC_BUFFER_H_

#include <vector>
#include <algorithm>
#include <cinttypes>

#include "fl_accessor.h"

template <typename T>
class FLStaticBuffer {
        public:
                FLStaticBuffer() = delete;
		FLStaticBuffer(unsigned int size);

		struct Iterator {
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;

			Iterator(pointer buffer, uint8_t* used, pointer end) {
				buffer_ptr = buffer;
				used_ptr = used;
				end_ptr = end;
				while (!(*used_ptr) && buffer_ptr != end_ptr) {
					buffer_ptr++;
					used_ptr++;
				}
			}

			reference operator*() const { return *buffer_ptr; }
			pointer operator->() { return buffer_ptr; }

			Iterator& operator++() {
				buffer_ptr++;
				used_ptr++;

				while (!(*used_ptr) && buffer_ptr != end_ptr) {
					buffer_ptr++;
					used_ptr++;
				}

				return *this;
			}

			Iterator& operator++(int) {
				Iterator tmp = *this;
			
				++(*this);

				return tmp;
			}

			friend bool operator== (const Iterator& a, const Iterator& b) { 
				return a.buffer_ptr == b.buffer_ptr;
			}

			friend bool operator!= (const Iterator& a, const Iterator& b) { 
				return a.buffer_ptr != b.buffer_ptr;
			}

			private:

			pointer buffer_ptr;
			pointer end_ptr;
			uint8_t* used_ptr;
		};

                FLAccessor<T> create();
                size_t size();
                void destroy(int handle);
                void clear();
                std::vector<T> &buf();

                T operator [](int i) const { return buffer[i]; }
                T & operator [](int i) { return buffer[i]; }

		Iterator begin() { return Iterator(buffer.data(), used.data(), &(buffer.data()[buffer.size()])); }
		Iterator end() { return Iterator(&(buffer.data()[buffer.size()]), &(used.data()[used.size()]), &(buffer.data()[buffer.size()])); }

        private:
                std::vector<T> buffer;
                std::vector<uint8_t> used;	// avoiding bool for now to allow regular vector access
                unsigned int last_added_pos;
		unsigned int buffer_size;

};

template <typename T>
FLStaticBuffer<T>::FLStaticBuffer(unsigned int size) {
	buffer.resize(size);
	used.resize(size);

	clear();
}

template <typename T>
FLAccessor<T> FLStaticBuffer<T>::create() {
	FLAccessor<T> ret = FLAccessor<T>();
	int pos = last_added_pos + 1;
	int end = last_added_pos;
	
	if ((unsigned int) pos >= buffer.size()) {
		pos = 0;
	}

	do {
		if (!used[pos]) {
			last_added_pos = pos;
			ret.create(pos, this);
			used[pos] = true;
			buffer_size += 1;
			break;
		}

		pos += 1;

		if ((unsigned int) pos >= buffer.size()) {
			pos = 0;
		}
	} while (pos != end);

	return ret;
}

template <typename T>
void FLStaticBuffer<T>::destroy(int handle) {
	if (handle >= 0 && handle < (int) buffer.size() && used[handle]) {
		used[handle] = false;
		buffer_size -= 1;
	}
}

template <typename T>
void FLStaticBuffer<T>::clear() {
	std::fill(used.begin(), used.end(), false);
	last_added_pos = 0;
	buffer_size = 0;
}

template <typename T>
size_t FLStaticBuffer<T>::size() {
	return buffer_size;
}

template <typename T>
std::vector<T> &FLStaticBuffer<T>::buf() {
	return buffer;
}

#endif
