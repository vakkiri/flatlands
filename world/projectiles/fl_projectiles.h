/*
 * 	fl_projectiles.h
 *
 * 	Header for projectile class and includes.
 *
 */

#ifndef FL_PROJECTILE_H_
#define FL_PROJECTILE_H_

#include "../../rendering/animated_object.h"

class FLProjectile : public FLGameObject {
	public:
		FLProjectile() = delete;
		FLProjectile( 	float x, 
				float y, 
				float w, 
				float h,
				float vx,
			       	float vy, 
				int damage, 
				FLAnimatedObjectParams animation_params );

		virtual ~FLProjectile();

		void on_collision( FLCollider* obj );
		virtual void per_frame_update();
	protected:
		int damage;
		int life;
};


class FLReepProjectile : public FLProjectile {
	public:
		FLReepProjectile( float x, float y, float vx, float vy );
};


#endif
