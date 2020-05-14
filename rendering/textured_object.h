/*
 * 	textured_object.h
 *
 * 	Base abstract class for any object which will be rendered with a 
 * 	texture.
 *
 */

#ifndef TEXTURED_OBJECT_H_
#define TEXTURED_OBJECT_H_

#include <vector>
#include "../common/game_object.h"
#include "../common/basic_types.h"

class FLTexturedObject : virtual public FLGameObject {
	public:
		FLTexturedObject();
		FLTexturedObject( float x, float y, float w, float h );

		virtual ~FLTexturedObject() {};

		virtual float s();
		virtual float t();

		void set_st( float s, float t );

		void set_reverse( bool reverse );
		bool reversed();
		
		void set_visible( bool visible );	
		bool is_visible();
	protected:
		float _s;
		float _t;
		bool reverse;
		bool visible;
};

#endif

