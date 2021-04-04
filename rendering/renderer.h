/*
 *	rendering.h
 *
 * 	Rendering utilities/functions.
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#define RESTART 	0xFFFF
#define SCREEN_WIDTH	1000
#define SCREEN_HEIGHT	600

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "../common/basic_types.h"
#include "fl_camera.h"
#include "fl_colored_poly_shader.h"
#include "fl_framebuffer_shader.h"
#include "fl_textured_rect_shader.h"
#include "fl_text_shader.h"

class FLAnimatedObject;
class FLRenderable;
class FLTexturedSurface;
class FLTextureSurface;
class FLTextSurface;
class FLDistortionSurface;
class FLWorldSurface;
class FLTexturedObject;

struct texture;

class Renderer {
  private:
	Renderer() : window(NULL), context(0){};

	// Disallow copying
	Renderer(Renderer const &) = delete;
	Renderer &operator=(Renderer const &) = delete;

	// Methods
	void clear_null_renderables();
	void render();

	// Member variables
	SDL_Window *window;
	SDL_GLContext context;
	std::vector<FLRenderable *> world_renderables;
	std::vector<FLRenderable *> custom_renderables;
	std::vector<FLRenderable *> background_renderables;
	std::vector<FLRenderable *> ui_renderables;

	// Shaders
	FLTexturedRectShader textured_rect_shader;
	FLColoredPolyShader colored_poly_shader;
	FLFramebufferShader framebuffer_shader;
	FLFramebufferShader water_shader;
	FLTextShader text_shader;

	// Surfaces
	FLWorldSurface *world_surface;
	FLWorldSurface *scenery_surface;
	FLTexturedSurface *tilemap_bg_surface;
	FLTexturedSurface *tilemap_fg_surface;
	FLTexturedSurface *background_surface;
	FLTexturedSurface *framebuffer_surface;
	FLTextSurface *text_surface;

	// Private methods
	bool init_sdl();
	bool init_window();
	bool init_gl();
	bool init_shaders();

	void prepare_to_render();
	void flip_framebuffer();
	void render_to_screen();

  protected:
	unsigned int screen_height;
	unsigned int screen_width;
	GLuint framebuffer;

	GLuint alt_rendered_texture;
	GLuint main_rendered_texture;
	GLuint *current_rendered_texture;

	texture *framebuffer_texture;
	texture *alt_framebuffer_texture;

	glm::mat4 projection_matrix;
	FLCamera world_camera;
	
	glm::mat4 ui_camera;
	glm::mat4 framebuffer_camera;
	glm::mat4 background_camera;

	void update_animations();
	void remove_null_objects();

  public:
	// Public methods
	texture *screen_texture();
	glm::mat4 get_projection_matrix();
	point screen_pos(float x, float y);

	FLCamera* get_world_camera();

	unsigned int get_screen_width();
	unsigned int get_screen_height();

	void add_to_world(FLTexturedObject *obj);
	void add_custom(FLRenderable *obj);
	void add_to_world(FLTexturedObject *obj, bool scenery);
	void remove_from_world(FLTexturedObject *obj);
	void remove_custom(FLRenderable *obj);
	void render_and_swap();

	void close();
	void clear();

	FLTexturedRectShader *get_textured_rect_shader();
	FLColoredPolyShader *get_colored_poly_shader();
	FLFramebufferShader *get_water_shader();
	FLTextShader *get_text_shader();
	FLWorldSurface *get_world_surface();
	FLTexturedSurface *get_tilemap_bg_surface();
	FLTextSurface *get_text_surface();
	FLTexturedSurface *get_tilemap_fg_surface();

	static Renderer &getInstance() {
		static Renderer instance;
		return instance;
	}

	bool init();
	void init_surface_textures();
};

// TODO: this should eventually replace the entire Renderer class
namespace FLRenderer {
	void init();
	void render();
	FLCamera* get_world_camera();
	FLTextureSurface *get_texture_surface(std::string name);
	FLTexturedRectShader& get_texture_shader();
}

#endif
