/*
 * 	player.h
 *
 * 	Base player class for a character who is sensitive to physics
 * 	and user input.
 *
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "../../resources/fl_resources.h"
#include "../../rendering/animated_object.h"
#include "../physics_object.h"

class FLTexturedSurface;
struct texture;

class FLPlayer : public FLAnimatedObject, public FLPhysicsObject {
	public:
		FLPlayer();

		void set_texture( texture *tex );
		void update_surface();

		// movement methods
		void jump();
		void move_left();
		void move_right();

		virtual void update_physics();
		virtual void update_camera();

		virtual void hold_jump();
		virtual void release_jump();
	protected:
		virtual void apply_gravity();
		virtual void bind_actions();
		virtual void bound_velocity();
		bool jump_held;
	private:
		FLTexturedSurface* surface;
};

#endif

