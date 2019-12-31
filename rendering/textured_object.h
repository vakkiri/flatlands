/*
 * 	textured_object.h
 *
 * 	Base abstract class for any object which will be rendered with a 
 * 	texture.
 *
 */

#ifndef TEXTURED_OBJECT_H_
#define TEXTURED_OBJECT_H_

class FLTexturedObject {
	public:
		FLTexturedObject();
		virtual float x() = 0;
		virtual float y() = 0;
		virtual float w() = 0;
		virtual float h() = 0;
		virtual float s();
		virtual float t();

		void set_st( float s, float y );

	protected:
		float _s;
		float _t;
};

#endif

