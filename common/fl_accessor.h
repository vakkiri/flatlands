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

template <typename T>
class FLStaticBuffer;

template <typename T>
class FLAccessor {
	public:
		FLAccessor();
		FLAccessor(int handle, FLStaticBuffer<T>* buf);

		void destroy();
		void create(int handle, FLStaticBuffer<T>* buf);
		bool null();
		T& get();
		
		T* operator->() { return null() ? nullptr : &((*buf)[handle]); }
	private:
		int handle;
		FLStaticBuffer<T>* buf;
};

template <typename T>
FLAccessor<T>::FLAccessor() : handle(-1), buf(nullptr) {}

template <typename T>
FLAccessor<T>::FLAccessor(int handle, FLStaticBuffer<T>* buf) : handle(handle), buf(buf) {}

template <typename T>
void FLAccessor<T>::destroy() { buf->destroy(handle); }

template <typename T>
void FLAccessor<T>::create(int handle, FLStaticBuffer<T>* buf) {
	this->handle = handle;
	this->buf = buf;
}

template <typename T>
T& FLAccessor<T>::get() { return (*buf)[handle]; }

template <typename T>
bool FLAccessor<T>::null() { return buf == nullptr || handle < 0; }

#endif

