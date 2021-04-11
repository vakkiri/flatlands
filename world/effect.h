/*
 * 	effect.h
 *
 * 	In-game animated effects which automatically disappear after
 * 	some time. Ie. smoke etc.
 *
 */

#ifndef EFFECT_H_
#define EFFECT_H_

#include <string>

namespace FLEffects {
	void create(float x, float y, std::string collection, unsigned int ticks_per_frame);
}

#endif
