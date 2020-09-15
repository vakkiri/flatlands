/*
 * 	portal.h
 *
 * 	Portals that take you to other positions/levels. Ie. doors.
 *
 */

#ifndef FL_PORTAL_H_
#define FL_PORTAL_H_

#include "../common/game_object.h"

class FLPortal : virtual public FLGameObject {
	public:
		FLPortal(float x, float y, float w, float h, float destx, float desty,
				int dest_level);
		~FLPortal();
		virtual void interact();
	protected:
		float destx;
		float desty;
		int dest_level;
};

void clear_portals();

#endif

