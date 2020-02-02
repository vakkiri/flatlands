/*
 * 	angel.h
 *
 * 	Base class for "angel's" ie. animated animes with
 * 	AOE shader effects and possibly attacks.
 *
 */

#ifndef ANGEL_H_
#define ANGEL_H_

#include "../rendering/animated_object.h"
#include "../world/world_object.h"

class NVAngel : public FLAnimatedObject, public FLWorldObject {
	public:
		NVAngel( float x, float y );
		virtual ~NVAngel();

};


#endif


