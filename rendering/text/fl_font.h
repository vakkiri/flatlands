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

struct texture;

struct fl_character {
	float s;
	float t;
};

class FLBitmapFont {
	public:
		FLBitmapFont() = delete;
		FLBitmapFont(std::string name, int char_w, int char_h);
	protected:
		void initialize();
		texture* tex;
		std::unordered_map<char, fl_character> characters;
		int char_w;
		int char_h;
};

FLBitmapFont& get_font(std::string name);

void fl_add_font(std::string name);
void fl_add_font(std::string name, int char_w, int char_h);

#endif

