/*
 * 	fl_ui_manager.h
 *
 * 	Class for managing all active UI elements.
 *	This includes both input and rendering of elements.
 *
 */

#ifndef FL_UI_MANAGER_H_
#define FL_UI_MANAGER_H_

#include <vector>

class FLUIElement;
class FLColoredSurface;
class FLTexturedSurface;

class FLUIManager {
	private:
		FLUIManager();
		FLUIManager(FLUIManager const&) = delete;
		FLUIManager& operator=(FLUIManager const&) = delete;
	public:
		static FLUIManager& getInstance() {
			static FLUIManager instance;
			return instance;
		}

		~FLUIManager();

		void init();

		// rendering	
		virtual void render();

		// input handling
		virtual void handle_up();
		virtual void handle_down();
		virtual void handle_left();
		virtual void handle_right();
		virtual void handle_accept();
		virtual void handle_reject();

		// element access
		void add_element( FLUIElement* element );
		void remove_element( FLUIElement* element );
		void set_active_element( FLUIElement* element );

	protected:
		FLUIElement* active_element;
		std::vector<FLUIElement*> elements;

		FLColoredSurface* primitive_surface;
		FLTexturedSurface* image_surface;
		FLTexturedSurface* text_surface;
};

#endif

