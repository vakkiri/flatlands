/*
 *	init_rendering.cpp
 *
 */

#include <glm/ext.hpp>
#include <iostream>

#include "../common/basic_types.h"
#include "../logging/logging.h"
#include "../resources/fl_resources.h"
#include "fl_text_surface.h"
#include "rendered_surface.h"
#include "renderer.h"
#include "texture.h"
#include "textured_object.h"
#include "world_surface.h"

#define PRIMITIVE_RESTART 65535

bool Renderer::init_shaders() {
	GLenum error;

	log_progress("Initializing shaders");

	projection_matrix =
		glm::ortho<GLfloat>(0.0, screen_width, screen_height, 0.0, 1.0, -1.0);

	if ((error = glGetError()) != GL_NO_ERROR) {
		log_error("Could not set projection matrix");
		std::cout << "Error: " << error;
		return false;
	}

	framebuffer_camera = glm::mat4(1.0);
	background_camera = glm::mat4(1.0);
	ui_camera = glm::mat4(1.0);

	// we have to flip the framebuffer camera since everything is
	// rendered upside down
	framebuffer_camera[1][1] = -1.0;
	framebuffer_camera[3][1] = (float)screen_height;
	world_camera.set_scale(2.0);
	ui_camera[0][0] = 2.0;
	ui_camera[1][1] = 2.0;
	background_camera[0][0] = 2.0;
	background_camera[1][1] = 2.0;

	textured_rect_shader.create_program("textured_rect_shader");
	textured_rect_shader.bind();
	textured_rect_shader.set_projection(projection_matrix);
	textured_rect_shader.set_camera(world_camera.mat());
	textured_rect_shader.update_pc_matrix();

	text_shader.create_program("text_shader");
	text_shader.bind();
	text_shader.set_projection(projection_matrix);
	text_shader.set_camera(ui_camera);
	text_shader.update_pc_matrix();

	colored_poly_shader.create_program("colored_poly_shader");
	colored_poly_shader.bind();
	colored_poly_shader.set_projection(projection_matrix);
	colored_poly_shader.set_camera(background_camera);
	colored_poly_shader.update_pc_matrix();

	framebuffer_shader.create_program("framebuffer-shader");
	framebuffer_shader.bind();
	framebuffer_shader.set_projection(projection_matrix);
	framebuffer_shader.set_camera(framebuffer_camera);
	framebuffer_shader.update_pc_matrix();

	water_shader.create_program("water-shader");
	water_shader.bind();
	water_shader.set_projection(projection_matrix);
	water_shader.set_camera(world_camera.mat());
	water_shader.update_pc_matrix();

	return true;
}

bool Renderer::init_gl() {
	GLenum error;
	log_progress("Initializing OpenGL");

	// Set viewport
	glViewport(0.f, 0.f, (float)screen_width, (float)screen_height);

	glClearColor(0.8f, 0.75f, 0.775f, 1.0f);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(PRIMITIVE_RESTART);

	if ((error = glGetError()) != GL_NO_ERROR) {
		log_error("Could not initialize OpenGL");
		std::cout << "Error: " << error;
		return false;
	}

	// generate and bind framebuffer
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	if ((error = glGetError()) != GL_NO_ERROR) {
		log_error("Could not bind frame buffer");
		std::cout << "Error: " << error;
		return false;
	}

	// generate the textures to render to
	glGenTextures(1, &main_rendered_texture);
	glBindTexture(GL_TEXTURE_2D, main_rendered_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_width, screen_height, 0,
				 GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenTextures(1, &alt_rendered_texture);
	glBindTexture(GL_TEXTURE_2D, alt_rendered_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_width, screen_height, 0,
				 GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
						   main_rendered_texture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		log_error("Could not initialize frame buffer");
		std::cout << "Error: " << error;
		return false;
	}

	return true;
}

bool Renderer::init_window() {
	log_progress("Creating window");

	window = SDL_CreateWindow(
		"shift", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width,
		screen_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (window == nullptr) {
		log_error(SDL_GetError());
		return false;
	}

	context = (SDL_GL_CreateContext(window));
	if (context == NULL) {
		log_error(SDL_GetError());
		return false;
	}

	if (SDL_GL_SetSwapInterval(1) < 0)
		log_warning(SDL_GetError());

	return true;
}

bool Renderer::init_sdl() {
	log_progress("Initializing SDL");

	// TODO: since this initializes more than graphics, we should have an init
	// external to /rendering
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER |
				 SDL_INIT_EVENTS) < 0) {
		log_error(SDL_GetError());
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
						SDL_GL_CONTEXT_PROFILE_CORE);

	return true;
}

bool Renderer::init() {
	log_progress("Initializing rendering engine");
	screen_width = SCREEN_WIDTH;
	screen_height = SCREEN_HEIGHT;

	if (!init_sdl()) {
		log_error("Could not initialize SDL");
		return false;
	}
	if (!init_window()) {
		log_error("Could not create window");
		return false;
	}
	if (!init_gl()) {
		log_error("Could not initialize OpenGL");
		return false;
	}
	if (!init_shaders()) {
		log_error("Could not initialize shaders");
		return false;
	}

	log_progress("Settings surface textures");

	world_surface = new FLWorldSurface();
	scenery_surface = new FLWorldSurface();
	tilemap_bg_surface = new FLTexturedSurface();
	tilemap_fg_surface = new FLTexturedSurface();
	background_surface = new FLTexturedSurface();
	framebuffer_surface = new FLTexturedSurface();
	text_surface = new FLTextSurface();

	world_surface->set_shader(&textured_rect_shader);
	scenery_surface->set_shader(&textured_rect_shader);
	tilemap_bg_surface->set_shader(&textured_rect_shader);
	tilemap_fg_surface->set_shader(&textured_rect_shader);
	background_surface->set_shader(&textured_rect_shader);
	framebuffer_surface->set_shader(&framebuffer_shader);
	text_surface->set_shader(&text_shader);

	world_renderables.push_back(tilemap_bg_surface);
	world_renderables.push_back(tilemap_fg_surface);
	world_renderables.push_back(scenery_surface);
	world_renderables.push_back(world_surface);
	background_renderables.push_back(background_surface);

	return true;
}

void Renderer::init_surface_textures() {
	world_surface->set_tex(FLResources::getInstance().get_image("world"));
	scenery_surface->set_tex(FLResources::getInstance().get_image("scenery"));
	tilemap_bg_surface->set_tex(FLResources::getInstance().get_image("tiles"));
	tilemap_fg_surface->set_tex(FLResources::getInstance().get_image("tiles"));

	// background shape and texture
	background_surface->set_tex(FLResources::getInstance().get_image("background1"));

	// TODO: set background to repeat tile
	FLTexturedObject *background_shape =
		new FLTexturedObject(0, 0, SCREEN_WIDTH * 8, SCREEN_HEIGHT * 8);
	background_surface->update_buffers(background_shape);
	delete background_shape;

	// set texture of framebuffer based surfaces

	framebuffer_texture = new texture{(float)screen_width, (float)screen_height,
									  main_rendered_texture};
	alt_framebuffer_texture = new texture{
		(float)screen_width, (float)screen_height, alt_rendered_texture};

	framebuffer_surface->set_tex(framebuffer_texture);


	// set shape of framebuffer based surfaces
	FLTexturedObject *framebuffer_shape =
		new FLTexturedObject(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	framebuffer_surface->update_buffers(framebuffer_shape);

	delete framebuffer_shape;

	// set the font of the text surface (the font contains a texture)
	text_surface->set_font("fl-plain-mono");
}

glm::mat4 Renderer::get_projection_matrix() { return projection_matrix; }
