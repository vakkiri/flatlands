/*
 * 	tile.h
 *
 * 	Basic textured tile structure.
 *
 */

#ifndef TILE_H_
#define TILE_H_

#include "../common/common.h"

struct tile {
	rect bounds;
	unsigned int u;
	unsigned int v;
};

#endif
