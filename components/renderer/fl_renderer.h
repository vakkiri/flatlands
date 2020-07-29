/*
 * 	fl_renderer.h
 *
 * 	component for rendering of objects
 *
 */

#ifndef FL_RENDERER_H_
#define FL_RENDERER_H_

#include <SDL2/SDL_opengl.h>
#include "types.h"

class FLShader;
class FLRendererSurface;

class FLRenderer {
	public:
		FLRenderer();

		void render();
		void enqueue();

		void set_texture(texture *tex);
		void set_st(float s, float t);
		void set_size(float w, float h);
		void set_dynamic(bool dynamic);
		void set_layer(int layer);
		void set_shader(FLShader *shader);
		void set_surface(FLRendererSurface *surface);
		void kill();
		bool is_alive();

		int get_layer() const;
		bool is_dynamic() const;
	protected:
		fl_frame frame;
		//TODO: animator...
		FLShader* shader;
		FLRendererSurface* surface;
		bool dynamic;
		int layer;
		bool alive;
};

struct compare_renderers {
	bool operator()(const FLRenderer* left, const FLRenderer* right) const {
		return left->get_layer() < right->get_layer();
	}
};

#endif


