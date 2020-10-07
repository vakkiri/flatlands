/*
 * 	xp_orb.h
 *
 * 	Flying orbs that give you experience.
 *
 */

#ifndef FL_XPORB_H_
#define FL_XPORB_H_

#include "../../common/game_object.h"

class FLXPOrb : public FLGameObject {
	public:
		FLXPOrb() = delete;
		FLXPOrb(float x, float y);
		virtual ~FLXPOrb();

		virtual void per_frame_update();

	protected:
		int ticks_since_created;
		point distance_from_player();
};

void clear_xp_orbs();

#endif

