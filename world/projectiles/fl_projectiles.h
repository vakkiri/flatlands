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
	FLProjectile(float x, float y, float w, float h, float vx, float vy,
				 int damage, FLAnimatedObjectParams animation_params);
	FLProjectile(float x, float y, float w, float h, float vx, float vy, int damage);
	virtual ~FLProjectile();

	void on_collision(FLCollider *obj);
	virtual void per_frame_update();

  protected:
	bool stationary();
	int damage;
	int life;
	bool hits_player;
	bool die_on_stop;
};

class FLReepProjectile : public FLProjectile {
  public:
	FLReepProjectile(float x, float y, float vx, float vy);
};

class FLFusionProjectile : public FLProjectile {
	public:
		FLFusionProjectile(float x, float y, float vx, float vy);
		virtual ~FLFusionProjectile();
};

class FLAoeBurst : public FLProjectile {
	public:
		FLAoeBurst(float x, float y, float w, float h, int damage, bool hits_player);
};

#endif
