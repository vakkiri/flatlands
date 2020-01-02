/*
 * 	textured_object.h
 *
 * 	Base abstract class for any object which will be rendered with a 
 * 	texture.
 *
 */

#ifndef TEXTURED_OBJECT_H_
#define TEXTURED_OBJECT_H_

#include "../world/world_object.h"

class FLTexturedObject : virtual public FLWorldObject {
	public:
		FLTexturedObject();
		FLTexturedObject( float x, float y, float w, float h );
		virtual float s();
		virtual float t();

		void set_st( float s, float y );

	protected:
		float _s;
		float _t;
};

#endif

