/*
 * 	scenery.h
 *
 * 	Textured objects that are too large/awkwardly shaped/specific to fit into a tileset.
 *
 */

#ifndef SCENERY_H_
#define SCENERY_H_

#include "../common/game_object.h"
#include "../rendering/animated_object.h"

class FLScenery : virtual public FLGameObject {
	public:
		FLScenery(float x, float y, int type);
		virtual ~FLScenery();
};

#endif

