/*
 * 	fl_font.h
 *
 * 	class(es?) for holding fonts
 *
 */

#ifndef FL_FONT_H_
#define FL_FONT_H_

#include <string>
#include <unordered_map>

#include "../../common/basic_types.h"

struct texture;

struct fl_character {
	float x;
	float y;
	float s;
	float t;
	fl_color color;
};

class FLBitmapFont {
	public:
		FLBitmapFont() = delete;
		FLBitmapFont(std::string name, int char_w, int char_h);

		point& get_character(char c);
		texture* get_texture();

		int get_char_w();
		int get_char_h();	
	protected:
		void initialize();
		texture* tex;
		std::unordered_map<char, point> characters;
		int char_w;
		int char_h;
};

FLBitmapFont* get_font(std::string name);

void fl_add_font(std::string name);
void fl_add_font(std::string name, int char_w, int char_h);

#endif

