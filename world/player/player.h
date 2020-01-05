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
#include "../physics_object.h"

class FLTexturedSurface;
struct texture;

class FLPlayer : public FLAnimatedObject, public FLPhysicsObject {
	public:
		FLPlayer(FLTexturedSurface* surface);

		void set_texture( texture *tex );
		void update_surface();

		// movement methods
		void jump();
		void move_left();
		void move_right();

	protected:
		virtual void bind_actions();

	private:
		FLTexturedSurface* surface;
};

#endif

