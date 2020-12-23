/*
 * 	background.h
 *
 * 	Parallax/layered backgrounds.
 *
 */

#ifndef BACKGROUND_H_
#define BACKGROUND_H_

#include <string>
#include <vector>

struct bg_layer {
	std::string tex;
	float px;
	float py;
};

class FLBackground {
	public:
		FLBackground() {};

		void add_layer(std::string tex, float px, float py);

		std::vector<bg_layer>& get_layers();

	private:
		std::vector<bg_layer> layers;
};

void add_background(std::string name);
void add_background_layer(std::string name, std::string tex, float px, float py);
std::vector<bg_layer>& get_background_layers(std::string name);

#endif

