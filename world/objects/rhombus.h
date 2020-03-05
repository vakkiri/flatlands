/*
 * 	rhombus.h
 *
 * 	Another bullet hell enemy generator.
 *
 */

#ifndef RHOMBUS_H_
#define RHOMBUS_H_

#include "../../rendering/animated_object.h"
#include "../colliding_object.h"
#include "../dynamic_object.h"

class FLRhombus : virtual public FLCollidingObject, virtual public FLAnimatedObject, virtual public FLDynamicObject {
	public:
		FLRhombus( float x, float y );
		virtual ~FLRhombus();
		virtual void collide_with( FLPlayer *player );
		virtual float x();
		virtual float y();
		virtual void update();
	protected:
		float movement_radius;
		float speed;
		float phase;
		unsigned int offset;
};


class FLSmallrhombus : virtual public FLCollidingObject, virtual public FLAnimatedObject, virtual public FLDynamicObject {
	public:
		FLSmallrhombus( FLRhombus* parent );
		virtual ~FLSmallrhombus();
		virtual void collide_with( FLPlayer *player );

		virtual void update();
		virtual float x();
		virtual float y();
	protected:
		FLRhombus* parent;
		float movement_radius;
		float phase;
		float offset;
		float speed;
};

#endif

