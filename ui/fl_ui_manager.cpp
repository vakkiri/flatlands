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
	std::function<void(void)> down = std::bind(&FLUIManager::handle_down, &(FLUIManager::getInstance()) );
	std::function<void(void)> left = std::bind(&FLUIManager::handle_left, &(FLUIManager::getInstance()) );
	std::function<void(void)> right = std::bind(&FLUIManager::handle_right, &(FLUIManager::getInstance()) );
	std::function<void(void)> accept = std::bind(&FLUIManager::handle_accept, &(FLUIManager::getInstance()) );
	std::function<void(void)> reject = std::bind(&FLUIManager::handle_reject, &(FLUIManager::getInstance()) );

	FLInputHandler::getInstance().add_ui_action( FL_KEY_UP, FL_KEY_PRESSED, up );
	FLInputHandler::getInstance().add_ui_action( FL_KEY_DOWN, FL_KEY_PRESSED, down );
	FLInputHandler::getInstance().add_ui_action( FL_KEY_LEFT, FL_KEY_PRESSED, left );
	FLInputHandler::getInstance().add_ui_action( FL_KEY_RIGHT, FL_KEY_PRESSED, right );
	FLInputHandler::getInstance().add_ui_action( FL_KEY_ACTION1, FL_KEY_PRESSED, accept );
	FLInputHandler::getInstance().add_ui_action( FL_KEY_ACTION2, FL_KEY_PRESSED, reject );
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

