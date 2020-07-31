/*
 * 	fl_font.cpp
 *
 */

#include <iostream>
#include <unordered_map>
#include "fl_font.h"
#include "../../resources/fl_resources.h"
#include "../texture.h"

#define DEFAULT_W 5
#define DEFAULT_H 10

static std::unordered_map<std::string, FLBitmapFont*> fonts;

FLBitmapFont::FLBitmapFont(std::string name, int char_w, int char_h) {
	tex = FLResources::getInstance().get_image(name);
	this->char_w = char_w;
	this->char_h = char_h;
	initialize();
}

void FLBitmapFont::initialize() {
	char c = ' ';
	for (float t = 0; t < tex->h; t += char_h) {
		for (float s = 0; s < tex->w; s += char_w) {
			characters[c] = point{s, t};
			c++;
		}
	}
}

point& FLBitmapFont::get_character(char c) {
	point out = characters[' '];

	if (characters.find(c) != characters.end()) {
		out = characters[c];
	}

	return characters[c];
}

texture* FLBitmapFont::get_texture() {
	return tex;
}

int FLBitmapFont::get_char_w() {
	return char_w;
}

int FLBitmapFont::get_char_h() {
	return char_h;
}

FLBitmapFont* get_font(std::string name) {
	FLBitmapFont *font = nullptr;
	if (fonts.find(name) != fonts.end()) {
		font = fonts[name];
	} else {
		std::cout << "Warning: font not found: " << name << std::endl;
	}

	return font;
}

void fl_add_font(std::string name) {
	fl_add_font(name, DEFAULT_W, DEFAULT_H);
}

void fl_add_font(std::string name, int char_w, int char_h) {
	FLBitmapFont *new_font = new FLBitmapFont(name, char_w, char_h);
	fonts[name] = new_font;
}

