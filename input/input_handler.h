/*
 * 	input_handler.h
 *
 * 	Class for detecting input signals and taking appropriate
 * 	action, ie. calling the right methods.
 *
 */

#ifndef INPUT_HANDLER_H_
#define INPUT_HANDLER_H_

#include <SDL2/SDL.h>
#include <map>
#include <functional>
#include <vector>
#include <utility>

enum FLKey {
	FL_KEY_UP,
	FL_KEY_RIGHT,
	FL_KEY_DOWN,
	FL_KEY_LEFT,
	FL_KEY_ACTION1,
	FL_KEY_ACTION2,
	FL_KEY_ACTION3,
	FL_KEY_ACTION4,
	FL_KEY_ACTION5,
	FL_NUM_KEYS
};

enum FLKeyState {
	FL_KEY_RELEASED,	// true on first frame a key is "up"
	FL_KEY_PRESSED,	// true on the first frame a key is "down"
	FL_KEY_HELD,	// true every frame a key is "down"
	FL_KEY_NONE,	// true on any frame the key is up
	FL_NUM_KEY_STATES
};

class FLInputHandler {
	public:
		static FLInputHandler& getInstance() {
			static FLInputHandler instance;
			return instance;
		}

		bool input_loop();
		void init();

		void add_game_action( FLKey key, FLKeyState state, std::function<void(void)> func );
		void add_ui_action( FLKey key, FLKeyState state, std::function<void(void)> func );
	protected:
		void handle_keyboard_state();
		void update_key_states();

		SDL_Event event;
		const Uint8* keyboard_state;

		// translate SDL events to game keys (ie. "up", "jump" etc.)
		std::map<unsigned int, FLKey> key_map;

		std::map<FLKey, FLKeyState> key_states;
		// translate in-game key presses/states to functions
		std::map<std::pair<FLKey, FLKeyState>, std::vector<std::function<void(void)>>> game_action_map;
		std::map<std::pair<FLKey, FLKeyState>, std::vector<std::function<void(void)>>> ui_action_map;

	private:
		// Disallow copying/construction
		FLInputHandler(){};
		FLInputHandler(FLInputHandler const&) = delete;
		FLInputHandler& operator=(FLInputHandler const&) = delete;
};

#endif

