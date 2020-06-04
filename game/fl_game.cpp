/*
 * 	fl_game.cpp
 *
 */

#include "fl_game.h"

#include "../environment/fl_environment.h"

void FLGame::init() { _environment = new FLEnvironment(); }

void FLGame::start() { _environment->load_level(0); }

FLEnvironment *FLGame::environment() { return _environment; }
