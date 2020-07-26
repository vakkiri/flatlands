/*
 * 	fl_font.h
 *
 * 	class(es?) for holding fonts
 *
 */

#ifndef FL_FONT_H_
#define FL_FONT_H_

#include <string>

class FLBitmapFont {
	public:
		FLBitmapFont() = delete;
		FLBitmapFont(std::string name);
	protected:
		void initialize();
		std::string image_name;
};

FLBitmapFont& get_font(std::string name);

void fl_add_font(std::string name);
void fl_add_font(std::string name, int char_w, int char_h);

#endif

