/*
 * 	fl_audio.cpp
 *
 */

#include <SDL2/SDL_mixer.h>
#include <iostream>
#include "fl_audio.h"

bool init_audio() {
	bool success = true;

	if ( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
		std::cout << "Could not open audio mixer.\n";
		std::cout << Mix_GetError() << std::endl;
		success = false;
	}

	return success;
}

