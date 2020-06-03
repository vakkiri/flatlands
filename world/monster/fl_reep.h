/*
 * 	fl_reep.h
 *
 * 	A type of flying monster.
 *
 */

#ifndef FL_REEP_H_
#define FL_REEP_H_

#include "fl_monster.h"

class FLReep : public FLMonster {
	public:
		FLReep( float x, float y );

	protected:
		virtual void move();
		virtual void attack();
};

#endif

