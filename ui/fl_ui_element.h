/*
 * 	fl_ui_element.h
 *
 * 	Base class for elements of the game UI, such as dialogue boxes
 * 	or menus.
 *
 */

#ifndef FL_UI_ELEMENT_H_
#define FL_UI_ELEMENT_H_

#include "../common/basic_types.h"
#include <vector>

class FLUIElement {
  public:
	FLUIElement();
	FLUIElement(float x, float y);

	virtual ~FLUIElement();

	// methods to handle input while UI element is active
	virtual void right() {}
	virtual void left() {}
	virtual void up() {}
	virtual void down() {}
	virtual void accept() {}
	virtual void reject() {}

	// rendering
	virtual std::vector<std::vector<fl_colored_vertex>> &
	get_primitive_vertices();
	virtual std::vector<int> &get_textured_objects();

	//setters
	void set_pos(float x, float y);

  protected:
	// the element geometry
	point offset;
	std::vector<std::vector<fl_colored_vertex>> primitive_vertices;
	std::vector<int> textured_objects;
};

#endif
