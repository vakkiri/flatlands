/*
 * 	fl_accessor.h
 *
 * 	Access wrapper for objects in custom buffers.
 *
 *
 */

#ifndef FL_ACCESSOR_H_
#define FL_ACCESSOR_H_

#include <vector>

class FLStaticBuffer;

template <typename T>
class FLAccessor {
	public:
		FLAccessor() = delete;
		FLAccessor(int handle, FLStaticBuffer<T> *buf);

		void destroy();
	private:
		int handle;
		FLStaticBuffer<T> *buf;
};

template <typename T>
FLAccessor<T>::FLAccessor(int handle, FLStaticBuffer<T> *buf) {
	this->handle = handle;
	this->buf = buf;
}

template <typename T>
FLAccessor<T>::destroy() {
	buf->destroy(handle);
}

#endif

