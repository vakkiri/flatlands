/*
 * 	scenery.h
 *
 * 	Textured objects that are too large/awkwardly shaped/specific to fit into a tileset.
 *	May be animated in the future.
 */

#ifndef SCENERY_H_
#define SCENERY_H_

#include "common/fl_object.h"
#include "common/game_object.h"
#include "rendering/animated_object.h"

class FLScenery : virtual public FLGameObject {
	public:
		FLScenery(float x, float y, int type);
		virtual ~FLScenery();
};

void clear_scenery();

/*
namespace FLScenery {
	void create(float x, float y, int type);
	void clear();
}*/

#endif

