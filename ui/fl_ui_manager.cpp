/*
 * 	fl_ui_manager.cpp
 *
 */

#include <iostream>
#include <algorithm>

#include "fl_ui_manager.h"
#include "fl_ui_element.h"

#include "../game/fl_gamestate.h"
#include "../input/input_handler.h"

FLUIManager::FLUIManager() {
	active_element = nullptr;
}

FLUIManager::~FLUIManager() {
	while ( !elements.empty() ) {
		if ( elements.back() != nullptr )
			delete elements.back();
		elements.pop_back();
	}
}

void FLUIManager::init() {
	std::function<void(void)> up = std::bind(&FLUIManager::handle_up, &(FLUIManager::getInstance()) );

	FLInputHandler::getInstance().add_ui_action( FL_KEY_UP, FL_KEY_PRESSED, up );
}

void FLUIManager::handle_up() {
	// if there are no UI elements active, we should not be
	// in the UI mode
	if ( active_element == nullptr )
		set_game_state(FL_GAME_RUNNING);
	else
		active_element->up();
}

void FLUIManager::handle_down() {
	// if there are no UI elements active, we should not be
	// in the UI mode
	if ( active_element == nullptr )
		set_game_state(FL_GAME_RUNNING);
	else
		active_element->down();
}

void FLUIManager::handle_left() {
	// if there are no UI elements active, we should not be
	// in the UI mode
	if ( active_element == nullptr )
		set_game_state(FL_GAME_RUNNING);
	else
		active_element->left();
}

void FLUIManager::handle_right() {
	// if there are no UI elements active, we should not be
	// in the UI mode
	if ( active_element == nullptr )
		set_game_state(FL_GAME_RUNNING);
	else
		active_element->right();
}

void FLUIManager::handle_accept() {
	// if there are no UI elements active, we should not be
	// in the UI mode
	if ( active_element == nullptr )
		set_game_state(FL_GAME_RUNNING);
	else
		active_element->accept();
}

void FLUIManager::handle_reject() {
	// if there are no UI elements active, we should not be
	// in the UI mode
	if ( active_element == nullptr )
		set_game_state(FL_GAME_RUNNING);
	else
		active_element->reject();
}

void FLUIManager::add_element( FLUIElement* element ) {
	elements.push_back( element );
}

void FLUIManager::remove_element( FLUIElement* element ) {
	elements.erase(std::remove(elements.begin(), elements.end(), element), elements.end());
}

void FLUIManager::set_active_element( FLUIElement* element ) {
	if ( std::find(elements.begin(), elements.end(), element ) != elements.end() )
		active_element = element;
}

void FLUIManager::render() {
	for (FLUIElement* element : elements)
		element->render();
}

