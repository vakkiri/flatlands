/*
 * 	fl_audio.h
 *
 * 	Flatlands audio header.
 *
 */

#ifndef FL_AUDIO_H_
#define FL_AUDIO_H_

#include <string>

bool init_audio();
void play_sound(std::string name);
void start_sound(std::string name);
void stop_sound(std::string name);

#endif
