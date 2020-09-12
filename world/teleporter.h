/*
 * 	teleporter.h
 *
 * 	Teleporters that take you to other positions/levels. Sorta like portals, but with a UI/options.
 *
 */

#ifndef FL_TELEPORTER_H_
#define FL_TELEPORTER_H_

#include "../common/game_object.h"

class FLTeleporter : virtual public FLGameObject {
	public:
		FLTeleporter(float x, float y);
		~FLTeleporter();
		virtual void interact();
};

void clear_teleporters();

#endif

