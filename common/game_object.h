/*
 * 	game_object.h
 *
 * 	Base type of bounded game objects.
 *
 */

#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <string>
#include <unordered_map>
#include "fl_shape.h"

class FLGameObject {
	public:
		FLGameObject();
		FLGameObject(float x, float y, float w, float h);
		~FLGameObject();

		virtual void set_x( float x );
		virtual void set_y( float y );
		virtual float x();
		virtual float y();
		virtual float w();
		virtual float h();

		void movex( float x );
		void movey( float y );
		void move( float x, float y);
		void move( point amt );
	protected:
		std::unordered_map<std::string, FLShape*> shapes;
};

#endif
