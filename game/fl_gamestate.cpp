/*
 * 	fl_gamestate.cpp
 */

#include "fl_gamestate.h"

FLGameState cur_state = FL_GAME_RUNNING;

FLGameState get_game_state() { return cur_state; }

void set_game_state(FLGameState new_state) { cur_state = new_state; }
