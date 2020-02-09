/*
 * 	fl_gamestate.h
 *
 * 	functionality for keeping track of the current game state.
 *
 */

#ifndef GAMESTATE_H_
#define GAMESTATE_H_

enum FLGameState {
	FL_GAME_RUNNING,
	FL_GAME_PAUSED,
	FL_GAME_UI
};

FLGameState get_game_state();

#endif

