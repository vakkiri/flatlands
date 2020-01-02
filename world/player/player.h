/*
 * 	player.h
 *
 * 	Base player class for a character who is sensitive to physics
 * 	and user input.
 *
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "../../rendering/animated_object.h"

class FLTexturedSurface;
struct texture;

class FLPlayer : public FLAnimatedObject {
	public:
		FLPlayer(FLTexturedSurface* surface);

		void set_texture( texture *tex );
		void update_surface();

		float x() { return FLWorldObject::x(); }
		float y() { return FLWorldObject::y(); }
		float w() { return FLWorldObject::w(); }
		float h() { return FLWorldObject::h(); }
	private:
		FLTexturedSurface* surface;
};

#endif

