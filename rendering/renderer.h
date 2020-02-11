/*
 *	rendering.h
 *
 * 	Rendering utilities/functions.
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "fl_colored_poly_shader.h"
#include "fl_textured_rect_shader.h"
#include "fl_smoke_background_shader.h"
#include "fl_framebuffer_shader.h"

class FLAnimatedObject;
class FLRenderable;
class FLTexturedSurface;
class FLDistortionSurface;
class FLWorldSurface;
class FLTexturedObject;
struct texture;

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
		std::vector<FLRenderable*> world_renderables;
		std::vector<FLRenderable*> background_renderables;
		std::vector<FLRenderable*> ui_renderables;
		std::vector<FLAnimatedObject*> animated_objects;

		// Shaders
		FLTexturedRectShader textured_rect_shader;
		FLColoredPolyShader colored_poly_shader;
		FLSmokeBackgroundShader background_shader;
		FLFramebufferShader custom_shader;

		// Surfaces
		FLWorldSurface *world_surface;
		FLTexturedSurface *tilemap_surface;
		FLTexturedSurface *background_surface;
		FLDistortionSurface *background_distortion_surface;
		FLTexturedSurface *framebuffer_surface;

		// Private methods
		bool init_sdl();	
		bool init_window();	
		bool init_gl();	
		bool init_shaders();

	protected:
		unsigned int screen_height;
		unsigned int screen_width;
		GLuint framebuffer;
		GLuint alt_rendered_texture;
		GLuint main_rendered_texture;
		texture* framebuffer_texture;
		texture* alt_framebuffer_texture;

		glm::mat4 projection_matrix;
		glm::mat4 world_camera;
		glm::mat4 framebuffer_camera;
		glm::mat4 background_camera;

		void update_animations();
		void remove_null_objects();
	public:
		// Public methods
		glm::mat4 get_projection_matrix();
		void translate_world_camera( glm::vec3 translation );
		float world_camera_x();
		float world_camera_y();

		unsigned int get_screen_width();
		unsigned int get_screen_height();

		void add_to_world( FLTexturedObject* obj );
		void remove_from_world( FLTexturedObject* obj );
		void clear_world();
		void add_animated_object( FLAnimatedObject* object );
		void remove_animated_object( FLAnimatedObject* object );
		void render_and_swap();

		void close();
		void clear();

		FLTexturedRectShader* get_textured_rect_shader();
		FLColoredPolyShader* get_colored_poly_shader();
		FLWorldSurface *get_world_surface();
		FLTexturedSurface *get_tilemap_surface();

		static Renderer& getInstance() {
			static Renderer instance;
			return instance;
		}

		bool init();
		void init_surface_textures();
};

#endif
