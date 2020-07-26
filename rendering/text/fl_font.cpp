/*
 * 	fl_font.cpp
 *
 */

#include <iostream>
#include <unordered_map>
#include "fl_font.h"

#define DEFAULT_W 5
#define DEFAULT_H 10

static std::unordered_map<std::string, FLBitmapFont*> fonts;

FLBitmapFont::FLBitmapFont(std::string name) {
	image_name = name;
	initialize();
}

void FLBitmapFont::initialize() {
}

void fl_add_font(std::string name) {
	fl_add_font(name, DEFAULT_W, DEFAULT_H);
}

void fl_add_font(std::string name, int char_w, int char_h) {
	(void) name;
	(void) char_w;
	(void) char_h;
}

