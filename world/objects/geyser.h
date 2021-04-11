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
	FL_GEYSER_ACTIVE
};

namespace FLGeysers {
	void clear();
	void create(float x, float y);
}

#endif

