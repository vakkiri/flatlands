/*
 * 	fl_ui_manager.cpp
 *
 */

#include <algorithm>
#include <iostream>

#include "fl_ammobar.h"
#include "fl_chipbar.h"
#include "fl_healthbar.h"
#include "fl_ui_element.h"
#include "fl_ui_manager.h"

#include "../game/fl_gamestate.h"
#include "../input/input_handler.h"
#include "../rendering/fl_colored_poly_shader.h"
#include "../rendering/rendered_surface.h"
#include "../rendering/renderer.h"
#include "../rendering/textured_object.h"

FLUIManager::FLUIManager() { active_element = nullptr; }

FLUIManager::~FLUIManager() {
	while (!elements.empty()) {
		if (elements.back() != nullptr)
			delete elements.back();
		elements.pop_back();
	}

	delete primitive_surface;
	delete text_surface;
}

void FLUIManager::init() {
	// init rendering surface
	primitive_surface = new FLColoredSurface();
	image_surface = new FLTexturedSurface();
	text_surface = new FLTexturedSurface();

	primitive_surface->set_shader(
		Renderer::getInstance().get_colored_poly_shader());
	image_surface->set_shader(
		Renderer::getInstance().get_textured_rect_shader());
	text_surface->set_shader(
		Renderer::getInstance().get_textured_rect_shader());

	image_surface->set_tex("ui");

	// init keys
	std::function<void(void)> up =
		std::bind(&FLUIManager::handle_up, &(FLUIManager::getInstance()));
	std::function<void(void)> down =
		std::bind(&FLUIManager::handle_down, &(FLUIManager::getInstance()));
	std::function<void(void)> left =
		std::bind(&FLUIManager::handle_left, &(FLUIManager::getInstance()));
	std::function<void(void)> right =
		std::bind(&FLUIManager::handle_right, &(FLUIManager::getInstance()));
	std::function<void(void)> accept =
		std::bind(&FLUIManager::handle_accept, &(FLUIManager::getInstance()));
	std::function<void(void)> reject =
		std::bind(&FLUIManager::handle_reject, &(FLUIManager::getInstance()));

	FLInputHandler::getInstance().add_ui_action(FL_KEY_UP, FL_KEY_PRESSED, up);
	FLInputHandler::getInstance().add_ui_action(FL_KEY_DOWN, FL_KEY_PRESSED,
												down);
	FLInputHandler::getInstance().add_ui_action(FL_KEY_LEFT, FL_KEY_PRESSED,
												left);
	FLInputHandler::getInstance().add_ui_action(FL_KEY_RIGHT, FL_KEY_PRESSED,
												right);
	FLInputHandler::getInstance().add_ui_action(FL_KEY_ACTION3, FL_KEY_RELEASED,
												accept);
	FLInputHandler::getInstance().add_ui_action(FL_KEY_ACTION2, FL_KEY_RELEASED,
												reject);

	// Add some default elements
	add_element(new FLHealthbar);
	add_element(new FLAmmobar);
	add_element(new FLChipbar);
}

void FLUIManager::handle_up() {
	// if there are no UI elements active, we should not be
	// in the UI mode
	if (active_element == nullptr)
		set_game_state(FL_GAME_RUNNING);
	else
		active_element->up();
}

void FLUIManager::handle_down() {
	// if there are no UI elements active, we should not be
	// in the UI mode
	if (active_element == nullptr)
		set_game_state(FL_GAME_RUNNING);
	else
		active_element->down();
}

void FLUIManager::handle_left() {
	// if there are no UI elements active, we should not be
	// in the UI mode
	if (active_element == nullptr)
		set_game_state(FL_GAME_RUNNING);
	else
		active_element->left();
}

void FLUIManager::handle_right() {
	// if there are no UI elements active, we should not be
	// in the UI mode
	if (active_element == nullptr)
		set_game_state(FL_GAME_RUNNING);
	else
		active_element->right();
}

void FLUIManager::handle_accept() {
	// if there are no UI elements active, we should not be
	// in the UI mode
	if (active_element == nullptr)
		set_game_state(FL_GAME_RUNNING);
	else
		active_element->accept();
}

void FLUIManager::handle_reject() {
	// if there are no UI elements active, we should not be
	// in the UI mode
	if (active_element == nullptr)
		set_game_state(FL_GAME_RUNNING);
	else
		active_element->reject();
}

void FLUIManager::add_element(FLUIElement *element) {
	elements.push_back(element);
}

void FLUIManager::remove_element(FLUIElement *element) {
	// TODO: maintain list of active element history, and "move back" here
	if (active_element == element) {
		active_element = nullptr;
		set_game_state(FL_GAME_RUNNING);
	}

	elements.erase(std::remove(elements.begin(), elements.end(), element),
				   elements.end());
}

void FLUIManager::set_active_element(FLUIElement *element) {
	if (std::find(elements.begin(), elements.end(), element) != elements.end())
		active_element = element;
}

void FLUIManager::render() {
	// render primitives
	Renderer::getInstance().get_colored_poly_shader()->bind();

	primitive_surface->clear_verts();
	for (FLUIElement *element : elements) {
		std::vector<std::vector<fl_colored_vertex>> &vert_sets =
			element->get_primitive_vertices();
		for (std::vector<fl_colored_vertex> &vert_set : vert_sets) {
			primitive_surface->add_verts(vert_set);
		}
	}
	primitive_surface->update_buffers();
	primitive_surface->render();

	// render images
	Renderer::getInstance().get_textured_rect_shader()->bind();

	std::vector<FLTexturedObject *> textures_to_draw;
	for (FLUIElement *element : elements) {
		std::vector<int> new_textures =
			element->get_textured_objects();
		textures_to_draw.reserve(textures_to_draw.size() + new_textures.size());

		for (auto id : new_textures) {
			textures_to_draw.push_back(get_texturer(id));
		}
	}
	image_surface->update_buffers(textures_to_draw);
	image_surface->render();

	// render all text
}
