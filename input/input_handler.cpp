/*
 * 	input_handler.cpp
 *
 */

#include "input_handler.h"
#include "../game/fl_gamestate.h"

void FLInputHandler::init() {
	SDL_StartTextInput();

	keyboard_state = SDL_GetKeyboardState(NULL);

	// populate empty key state map
	for ( int key = 0; key < FL_NUM_KEYS; key++ )
		key_states[FLKey(key)] = FL_KEY_RELEASED;

	// populate some defaults
	// TODO: read defaults from an external file
	key_map[SDL_SCANCODE_UP] = FL_KEY_UP;
	key_map[SDL_SCANCODE_RIGHT] = FL_KEY_RIGHT;
	key_map[SDL_SCANCODE_DOWN] = FL_KEY_DOWN;
	key_map[SDL_SCANCODE_LEFT] = FL_KEY_LEFT;
	key_map[SDL_SCANCODE_A] = FL_KEY_ACTION1;
	key_map[SDL_SCANCODE_S] = FL_KEY_ACTION2;
	key_map[SDL_SCANCODE_D] = FL_KEY_ACTION3;
}

bool FLInputHandler::input_loop() {
	bool quit = false;

	update_key_states();
	handle_keyboard_state();

	while ( SDL_PollEvent ( &event ) != 0 ) {
		if ( event.type == SDL_QUIT )
			quit = true;
	}

	return quit;
}

void FLInputHandler::handle_keyboard_state() {
	FLKey key;
	FLKeyState state;
	std::pair<FLKey, FLKeyState> action;

	for ( int i = 0; i < FL_NUM_KEYS; i++ ) {
		key = (FLKey) i;
		state = key_states[key];
		action = std::make_pair(key, state);

		if ( get_game_state() == FL_GAME_RUNNING ) {
			for ( std::function<void(void)> f : game_action_map[action] )
				f();
		}
		else if ( get_game_state() == FL_GAME_UI ) {
			for ( std::function<void(void)> f : ui_action_map[action] )
				f();
		}
	}

}

void FLInputHandler::update_key_states() {
	for ( auto kv : key_map ) {
		bool held = keyboard_state[kv.first];

		if ( !held ) {
			if ( key_states[kv.second] == FL_KEY_PRESSED ||
			     key_states[kv.second] == FL_KEY_HELD )
				key_states[kv.second] = FL_KEY_RELEASED;
			else
				key_states[kv.second] = FL_KEY_NONE;
		}
		else if ( key_states[kv.second] == FL_KEY_RELEASED ||
			  key_states[kv.second] == FL_KEY_NONE )
			key_states[kv.second] = FL_KEY_PRESSED;
		else if ( key_states[kv.second] == FL_KEY_PRESSED )
			key_states[kv.second] = FL_KEY_HELD;
		// else: held -> held
	}
}

void FLInputHandler::add_game_action( FLKey key, FLKeyState state, std::function<void()> func ) {
	std::pair<FLKey, FLKeyState> action = std::make_pair(key, state);
	game_action_map[action].push_back(func);
}

void FLInputHandler::add_ui_action( FLKey key, FLKeyState state, std::function<void()> func ) {
	std::pair<FLKey, FLKeyState> action = std::make_pair(key, state);
	ui_action_map[action].push_back(func);
}
