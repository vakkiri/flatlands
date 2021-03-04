/*
 * 	fl_ui_group.h
 *
 * 	Container for groups of releated UI primitives.
 *
 */

#ifndef FL_UI_GROUP_H_
#define FL_UI_GROUP_H_

#include <string>
#include <unordered_map>
#include <vector>
#include "rendering/rendered_surface.h"
#include "rendering/fl_text_surface.h"

class FLUIBasicElement;
class FLUIVectorElement;
class FLUITextElement;
class FLUITextureElement;

class FLUIGroup {
	public:
		FLUIGroup() = delete;
		FLUIGroup(std::string name);
		~FLUIGroup();

		void add_vector(std::string name, FLUIVectorElement* vec);
		void add_texture(std::string name, FLUITextureElement* tex);
		void add_text(std::string name, FLUITextElement* text);

		void update();

		void handle_up();
		void handle_down();
		void handle_left();
		void handle_right();
		void handle_accept();
		void handle_reject();

		void set_active_element(FLUIBasicElement *element);

		void render();
	private:
		FLUIBasicElement *active_element;

		std::unordered_map<std::string, FLUIBasicElement*> elements;
		std::vector<FLUIVectorElement*> vectors;
		std::vector<FLUITextureElement*> textures;
		std::vector<FLUITextElement*> text;

		FLTexturedSurface texture_surface;
		FLColoredSurface vector_surface;
		FLTextSurface text_surface;

		std::string name;
};

#endif

