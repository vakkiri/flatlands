/* * 	fl_chipbar.cpp
 */

#include "fl_chipbar.h"
#include <iostream>
#include <string>

#include "../environment/fl_environment.h"
#include "../game/fl_game.h"
#include "../rendering/fl_text_surface.h"
#include "../rendering/textured_object.h"
#include "../rendering/renderer.h"
#include "../world/player/player.h"

#define X_POS 16
#define Y_POS 52

FLChipbar::FLChipbar() : FLUIElement(X_POS, Y_POS) {
	init_textures();
}

/* same as in fl_dialogue_box, this sort of thing should be refactored into a render() method */
std::vector<FLTexturedObject*>& FLChipbar::get_textured_objects() {
	// add text showing number of chips and fragments
	Renderer& r = Renderer::getInstance();
	FLTextSurface* text_surface = r.get_text_surface();
	std::string chips = std::to_string(FLGame::instance().environment()->player()->get_chips());
	std::string fragments = std::to_string(FLGame::instance().environment()->player()->get_fragments());

	
	for (unsigned int i = 0; i < chips.size(); ++i) {	
		text_surface->add_character(offset.x + 24 + 5 * i, offset.y + 4, chips[i]);
	}
	for (unsigned int i = 0; i < fragments.size(); ++i) {	
		text_surface->add_character(offset.x + 24 + 5 * i, offset.y + 22, fragments[i]);
	}

	return FLUIElement::get_textured_objects();
}

void FLChipbar::init_textures() {
	icon = new FLTexturedObject(offset.x, offset.y, 16, 16);
	icon->set_st(128, 0);

	textured_objects.push_back(icon);

	fragment_icon = new FLTexturedObject(offset.x, offset.y + 18, 16, 16);
	fragment_icon->set_st(128, 16);

	textured_objects.push_back(fragment_icon);
}

