/*
 * 	fl_game.cpp
 *
 */

#include "fl_game.h"

#include "../environment/fl_environment.h"

// FIXME: this should be from a config file instead
#define START_LEVEL 1

void FLGame::init() { _environment = new FLEnvironment(); }

void FLGame::start() { _environment->load_level(START_LEVEL); }

FLEnvironment *FLGame::environment() { return _environment; }
