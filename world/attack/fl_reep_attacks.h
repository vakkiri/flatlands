/*
 * 	fl_reep_attacks.h
 *
 * 	Attacks of the reep monster.
 *
 */

#ifndef FL_REEP_SECONDARY_H_
#define FL_REEP_SECONDARY_H_

#include "fl_attack.h"

class FLReepSecondary : public FLAttack {
	public:
		FLReepSecondary(float x, float y);
	protected:
		virtual void pre_attack();
		virtual void attack();
};

#endif

