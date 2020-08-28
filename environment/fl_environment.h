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
	void set_level_id(int id);
	int get_level_id();
	FLPlayer *player();
	FLTilemap *tilemap();

  protected:
	FLPlayer *_player;
	FLTilemap *_tilemap;
	int level_id;
	std::vector<FLGameObject *> objects;
};

#endif
