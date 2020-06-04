/*
 * 	fl_environment.h
 *
 * 	Class for distinct areas/levels.
 *
 */

#ifndef FL_ENVIRONMENT_H_
#define FL_ENVIRONMENT_H_

#include <vector>

#include "../common/common.h"

class FLTilemap;
class FLPlayer;

class FLEnvironment {
  public:
	FLEnvironment();
	~FLEnvironment();

	void load_level(int id);
	void add_object(FLGameObject *object);

	FLPlayer *player();
	FLTilemap *tilemap();

  protected:
	FLPlayer *_player;
	FLTilemap *_tilemap;
	std::vector<FLGameObject *> objects;
};

#endif
