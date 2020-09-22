/*
 * 	geyser.h
 *
 * 	geysers that shoot out hot.. stuff...
 *
 */

#ifndef GEYSER_H_
#define GEYSER_H_

#include "../../common/game_object.h"
#include "../../rendering/animated_object.h"

enum FLGeyserState {
	FL_GEYSER_IDLE,
	FL_GEYSER_PRE_BURST
};

class FLGeyser : virtual public FLGameObject {
	public:
		FLGeyser(float x, float y);
		virtual ~FLGeyser();
		virtual void per_frame_update();
	protected:
		FLGeyserState state;
		int tick;
};

void clear_geysers();

#endif

