/*
 * 	fl_ui_element.h
 *
 * 	Base class for elements of the game UI, such as dialogue boxes
 * 	or menus.
 *
 */

#ifndef FL_UI_ELEMENT_H_
#define FL_UI_ELEMENT_H_

#include <vector>
#include "../common/basic_types.h"

class FLUIElement {
	public:
		FLUIElement();
		FLUIElement( float x, float y );

		// methods to handle input while UI element is active
		virtual void right(){}
		virtual void left(){}
		virtual void up(){}
		virtual void down(){}
		virtual void accept(){}
		virtual void reject(){}

		// rendering
		std::vector<fl_colored_vertex>& get_vertices();

	protected:
		// the element geometry
		point offset;
		std::vector<fl_colored_vertex> vertices;
};

#endif

