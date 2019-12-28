/*
 *	rendering.h
 *
 * 	Rendering utilities/functions.
 */

#ifndef RENDERING_H_
#define RENDERING_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class FLRenderable;

class Renderer {
	private:
		Renderer() : 	window(NULL),
				context(0)
		{};

		// Disallow copying
		Renderer(Renderer const&) = delete;
		Renderer& operator=(Renderer const&) = delete;

		// Methods
		void clear_null_renderables();
		void render();

		// Member variables
		SDL_Window* window;
		SDL_GLContext context;
		std::vector<FLRenderable*> renderables;

		// Private methods
		bool init_sdl();	
		bool init_window();	
		bool init_gl();	
		bool init_shaders();

	protected:
		unsigned int screen_height;
		unsigned int screen_width;
		glm::mat4 projection_matrix;
	public:
		// Public methods
		glm::mat4 get_projection_matrix();
		void add_renderable(FLRenderable* r);
		void render_and_swap();
		void close();

		static Renderer& getInstance() {
			static Renderer instance;
			return instance;
		}

		bool init();
};

bool init_rendering();

#endif
