/*
 * 	fl_audio.cpp
 *
 */

#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <unordered_map>
#include "fl_audio.h"
#include "../resources/fl_resources.h"

std::unordered_map<std::string, int> channel_map;	// maps sound names to their current channel

bool init_audio() {
	bool success = true;

	if ( Mix_OpenAudio( 44100, AUDIO_S16SYS, 2, 512 ) < 0 ) {
		std::cout << "Could not open audio mixer.\n";
		std::cout << Mix_GetError() << std::endl;
		success = false;
	}
	else {
		Mix_AllocateChannels(16);
	}

	return success;
}

void play_sound( std::string name ) {
	Mix_PlayChannel(-1, FLResources::getInstance().get_sound(name), 0);
}

void start_sound( std::string name ) {
	// we only start sounds that aren't already mapped to a channel
	if (channel_map.find(name) == channel_map.end() || channel_map[name] == -1)
		channel_map[name] = Mix_PlayChannel(-1, FLResources::getInstance().get_sound(name), -1);
}

void stop_sound( std::string name ) {
	if (channel_map.find(name) != channel_map.end() && channel_map[name] != -1) {
		Mix_HaltChannel(channel_map[name]);
		channel_map[name] = -1;
	}
}


