/*
 *	rendering.h
 *
 * 	Rendering utilities/functions.
 */

#ifndef RENDERING_H_
#define RENDERING_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <string>

class FLShader;

class Renderer {
	private:
		Renderer() : 	window(NULL),
				context(0),
				cur_shader(nullptr),
				gVBO(0),
				gIBO(0)
		{};

		// Disallow copying
		Renderer(Renderer const&) = delete;
		Renderer& operator=(Renderer const&) = delete;

		// Methods
		void render();

		// Member variables
		SDL_Window* window;
		SDL_GLContext context;
		GLuint gVBO;
		GLuint gIBO;

		FLShader *cur_shader;

		// Private methods
		bool init_sdl();	
		bool init_window();	
		bool init_gl();	
		bool init_shaders();
	protected:
		unsigned int screen_height;
		unsigned int screen_width;
	public:
		// Public methods
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
