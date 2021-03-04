/*
 * 	fl_ui.cpp
 *
 */

#include <iostream>
#include <functional>

#include "input/input_handler.h"
#include "fl_ui.h"
#include "fl_ui_group.h"

namespace FLUI {
	std::unordered_map<std::string, FLUIGroup*> element_groups;
	std::vector<FLUIGroup*> active_groups;

	// only the active group received input
	FLUIGroup* active_group() {
		return active_groups.back();
	}

	void push_active_group(FLUIGroup* group) {
		active_groups.push_back(group);
	}

	FLUIGroup *create_group(std::string name, bool active) {
		FLUIGroup* group = nullptr;
		if (element_groups.contains(name)) {
			std::cout << "Error: ui group " << name << " already exists\n";
		} else {
			group = new FLUIGroup(name);
			element_groups[name] = group;

			if (active) {
				push_active_group(group);
			}
		}
		
		return group;
	}

	void remove_group(std::string name) {
		if (element_groups.contains(name)) {
			FLUIGroup *group = element_groups[name];
			element_groups.erase(element_groups.find(name));

			// Note: this will break if somehow
			// elements are deleted out of order.
			// Thus, it is assumed that when a group 
			// which accepts input is not the most
			// recently pushed group, it will not change
			// state (ie. delete itself).
			if (active_groups.back() == group) {
				active_groups.pop_back();
			}

			delete group;
		} 
	}

	void handle_up() {
		if (active_group()) {
			active_group()->handle_up();
		}
	}

	void handle_down() {
		if (active_group()) {
			active_group()->handle_down();
		}
	}

	void handle_left() {
		if (active_group()) {
			active_group()->handle_left();
		}
	}

	void handle_right() {
		if (active_group()) {
			active_group()->handle_right();
		}
	}

	void handle_accept() {
		if (active_group()) {
			active_group()->handle_accept();
		}
	}

	void handle_reject() {
		if (active_group()) {
			active_group()->handle_reject();
		}
	}

	void init() {
		std::function<void(void)> up = handle_up;
		std::function<void(void)> down = handle_down;
		std::function<void(void)> left = handle_left;
		std::function<void(void)> right = handle_right;
		std::function<void(void)> accept = handle_accept;
		std::function<void(void)> reject = handle_reject;

		FLInputHandler& input = FLInputHandler::getInstance();

		input.add_ui_action(FL_KEY_UP, FL_KEY_PRESSED, up);
		input.add_ui_action(FL_KEY_DOWN, FL_KEY_PRESSED, down);
		input.add_ui_action(FL_KEY_LEFT, FL_KEY_PRESSED, left);
		input.add_ui_action(FL_KEY_RIGHT, FL_KEY_PRESSED, right);
		input.add_ui_action(FL_KEY_ACTION3, FL_KEY_RELEASED, accept);
		input.add_ui_action(FL_KEY_ACTION2, FL_KEY_RELEASED, reject);
	}

	void update() {
		for (auto [name, group] : element_groups) {
			group->update();
		}
	}
}

