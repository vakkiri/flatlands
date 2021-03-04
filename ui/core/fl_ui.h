/*
 * 	fl_ui.h
 *
 * 	Main UI namespace - holds/handles all UI objects.
 *
 */

#ifndef FL_UI_H_
#define FL_UI_H_

#include <string>

class FLUIGroup;

namespace FLUI {
	FLUIGroup *create_group(std::string name, bool active);
	void remove_group(std::string name);
	void init();
	void update();
}

#endif

