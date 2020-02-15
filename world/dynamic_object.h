/*
 * 	dynamic_object.h
 *
 * 	Objects which are updated each frame.
 *
 */

#ifndef DYNAMIC_OBJECT_H_
#define DYNAMIC_OBJECT_H_

#include <vector>

#include "world_object.h"

class FLDynamicObject : virtual public FLWorldObject {
	public:
		FLDynamicObject();
		virtual ~FLDynamicObject();
		virtual void update() = 0;
		void set_element_position( unsigned int position );
		bool is_zombie();
	protected:
		bool zombie;	// if the objects' life is over
	private:
		unsigned int element_position;
};

void add_dynamic_object( FLDynamicObject* object );
void update_dynamic_objects();
void clear_dynamic_objects();

#endif

