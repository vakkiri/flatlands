/*
 * 	fl_player_attacks.h
 *
 * 	Attacks of the players.
 *
 */

#ifndef FL_PLAYER_ATTACKS_H_
#define FL_PLAYER_ATTACKS_H_

#include "fl_attack.h"

class FLFusionPrimary : public FLAttack {
	public:
		FLFusionPrimary(float x, float y, FLHorizontalDirection hdir, FLVerticalDirection vdir);
	protected:
		virtual void attack();
};

#endif

