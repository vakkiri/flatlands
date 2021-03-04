/*
 * 	fl_ui_basic_element.h
 *
 * 	Base class which all UI elements should be derived from.
 *
 */

#ifndef FL_UI_BASIC_ELEMENT_H_
#define FL_UI_BASIC_ELEMENT_H_

class FLUIBasicElement {
	public:
		FLUIBasicElement();
		virtual ~FLUIBasicElement();

		virtual FLUIBasicElement* up();
		virtual FLUIBasicElement* down();
		virtual FLUIBasicElement* left();
		virtual FLUIBasicElement* right();
		virtual void update();
		virtual void accept();
		virtual void render();
		
	private:
		FLUIBasicElement* left_element;
		FLUIBasicElement* right_element;
		FLUIBasicElement* above_element;
		FLUIBasicElement* below_element;
};

#endif

