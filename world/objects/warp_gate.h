/*
 * 	warp_gate.h
 *
 * 	End of level warp gate.
 *
 */

#ifndef WARP_GATE_H_
#define WARP_GATE_H_

#include "../../rendering/animated_object.h"
#include "../colliding_object.h"

class FLWarpGate : virtual public FLAnimatedObject, virtual public FLCollidingObject {
	public:
		FLWarpGate( float x, float y );
		virtual ~FLWarpGate();
		virtual void collide_with();

};

#endif

