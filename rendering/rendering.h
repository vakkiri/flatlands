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

class Renderer {
	private:
		Renderer() : 	window(NULL),
				context(0),
				gProgramID(0),
				gVertexPos2DLocation(-1),
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
		GLuint gProgramID;
		GLint gVertexPos2DLocation;
		GLuint gVBO;
		GLuint gIBO;

		// Private methods
		bool init_sdl();	
		bool init_window();	
		bool init_gl();	
	public:
		// Public methods
		GLuint attach_shader(const GLchar* source[], GLenum shaderType);
		GLuint attach_shader(std::string path, GLenum shaderType);
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
