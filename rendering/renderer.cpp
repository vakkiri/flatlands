/*
 * 	render_main.cpp
 *
 * 	Main rendering functionality.
 *
 */

#include <fstream>
#include <glm/ext.hpp>
#include <iostream>
#include <algorithm>
#include <unordered_map>

#include "../common/basic_types.h"
#include "../environment/fl_environment.h"
#include "../game/fl_game.h"
#include "../logging/logging.h"
#include "../resources/fl_resources.h"
#include "../ui/fl_ui_manager.h"
#include "animated_object.h"
#include "background.h"
#include "renderable.h"
#include "rendered_surface.h"
#include "renderer.h"
#include "fl_text_surface.h"
#include "fl_texture_surface.h"
#include "world_surface.h"

namespace FLRenderer {
	std::unordered_map<std::string, FLTextureSurface*> texture_surfaces;

	FLTexturedRectShader texture_shader;

	glm::mat4 projection_matrix;
	FLCamera world_camera;

	FLCamera* get_world_camera() {
		return &world_camera;
	}

	void init_cameras() {
		// TODO: move this internally
		unsigned int screen_width = Renderer::getInstance().get_screen_width();
		unsigned int screen_height = Renderer::getInstance().get_screen_height();
        	projection_matrix = glm::ortho<GLfloat>(
				0.0,
				screen_width,
				screen_height,
				0.0, 1.0, -1.0
		);

		world_camera.set_scale(2.0);
	}

	void init_shaders() {
		texture_shader.create_program("textured_rect_shader");
		texture_shader.bind();
		texture_shader.set_projection(projection_matrix);
		texture_shader.update_pc_matrix();
	}

	void init_surfaces() {
		FLTextureSurface *world_surface = new FLTextureSurface(
			false,
			"world"
		);
		FLTextureSurface *bg_tile_surface = new FLTextureSurface(
			true,
			"tiles"
		);
		FLTextureSurface *fg_tile_surface = new FLTextureSurface(
			true,
			"tiles"
		);

		world_surface->set_camera(&world_camera);
		bg_tile_surface->set_camera(&world_camera);
		fg_tile_surface->set_camera(&world_camera);

		texture_surfaces["world"] = world_surface;
		texture_surfaces["bg_tiles"] = bg_tile_surface;
		texture_surfaces["fg_tiles"] = fg_tile_surface;
	}

	void init() {
		init_cameras();
		init_shaders();
		init_surfaces();
	}

	void render() {
		// TODO: create some kind of surface priorty so we can remove
		// individual surface calls/dynamically add custom shader surfs
		world_camera.update();

		texture_surfaces["bg_tiles"]->render();
		texture_surfaces["fg_tiles"]->render();
		texture_surfaces["world"]->render();
	}

	FLTextureSurface *get_texture_surface(std::string name) {
		if (texture_surfaces.find(name) != texture_surfaces.end()) {
			return texture_surfaces[name];
		} else {
			std::cout << "Error: Texture surface " << name << " does not exist.\n";
			return nullptr;
		}
	}

	FLTexturedRectShader &get_texture_shader() {
		return texture_shader;
	}
}

void Renderer::flip_framebuffer() {
	if (current_rendered_texture == &alt_rendered_texture) {
		current_rendered_texture = &main_rendered_texture;
	}
	else {
		current_rendered_texture = &alt_rendered_texture;
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
						   *current_rendered_texture, 0);
}

texture *Renderer::screen_texture() {
	// The texture returned will be the texture we are reading from, which
	// must be the opposite of the texture we are writing to.
	if (current_rendered_texture == &alt_rendered_texture) {
		return framebuffer_texture;
	}
	else {
		return alt_framebuffer_texture;
	}
}

void Renderer::render_to_screen() {
	// Make sure we get any changes to the framebuffer
	flip_framebuffer();

	// Bind our shader and ensure we are drawing to the screen
	framebuffer_shader.bind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	framebuffer_surface->set_tex(screen_texture());

	// Render the screen quad
	framebuffer_surface->render();
}

void Renderer::prepare_to_render() {
	clear_null_renderables();

	glClear(GL_COLOR_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	current_rendered_texture = &main_rendered_texture;
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
						   main_rendered_texture, 0);
}

void Renderer::render() {
	world_camera.update();

	prepare_to_render();

	// TODO: animation updating should be a separate component from rendering
	update_animations();

	// TODO: updating surfaces should be separate from rendering or its own
	// method
	world_surface->update_buffers();
	scenery_surface->update_buffers();

	// draw background to framebuffer
	textured_rect_shader.bind();
	background_surface->set_shader(&textured_rect_shader);

	std::string background = FLGame::instance().environment()->get_background();

	for (auto layer : get_background_layers("night1")) {
		background_camera[3][0] = world_camera.x() * layer.px; // parallax x
		background_camera[3][1] = world_camera.y() * layer.py; // parallax y
		textured_rect_shader.set_camera(background_camera);
		textured_rect_shader.update_pc_matrix();

		background_surface->set_tex(FLResources::getInstance().get_image(layer.tex));
		background_surface->render();
	}

	FLRenderer::render();

	// draw world -----------------------------------------
	textured_rect_shader.bind();
	textured_rect_shader.set_camera(world_camera.mat());
	textured_rect_shader.update_pc_matrix();

	for (FLRenderable *r : world_renderables)
		r->render();

	flip_framebuffer();

	// draw a copy of the screen ---
	framebuffer_shader.bind();
	framebuffer_surface->set_tex(screen_texture());
	framebuffer_surface->render();
	// ---

	// draw custom ... things.... like water
	water_shader.bind();
	water_shader.set_camera(world_camera.mat());
	water_shader.update_pc_matrix();

	for (FLRenderable *r : custom_renderables)
		r->render();

	// UI
	textured_rect_shader.bind();
	textured_rect_shader.set_camera(ui_camera);
	textured_rect_shader.update_pc_matrix();

	colored_poly_shader.bind();
	colored_poly_shader.set_camera(ui_camera);
	colored_poly_shader.update_pc_matrix();

	FLUIManager::getInstance().render();

	text_shader.bind();
	text_shader.set_camera(ui_camera);
	text_shader.update_pc_matrix();
	text_surface->render();

	// render framebuffer to screen
	
	render_to_screen();
}

void Renderer::render_and_swap() {
	render();
	SDL_GL_SwapWindow(window);
}

void Renderer::clear_null_renderables() {
	std::vector<int> nulls;

	for (unsigned int i = 0; i < world_renderables.size(); i++) {
		if (world_renderables[i] == nullptr)
			nulls.push_back(i);
	}

	for (int i : nulls) {
		world_renderables[i] = world_renderables.back();
		world_renderables.pop_back();
	}
}

point Renderer::screen_pos(float x, float y) {
	return point{	x + (world_camera.x() / world_camera.x_scale()), 
			y + (world_camera.y() / world_camera.y_scale())};
}

FLTexturedRectShader *Renderer::get_textured_rect_shader() {
	return &textured_rect_shader;
}

FLColoredPolyShader *Renderer::get_colored_poly_shader() {
	return &colored_poly_shader;
}

FLTextShader *Renderer::get_text_shader() {
	return &text_shader;
}

FLWorldSurface *Renderer::get_world_surface() { return world_surface; }

FLTexturedSurface *Renderer::get_tilemap_bg_surface() {
	return tilemap_bg_surface;
}

FLTextSurface *Renderer::get_text_surface() {
	return text_surface;
}

FLTexturedSurface *Renderer::get_tilemap_fg_surface() {
	return tilemap_fg_surface;
}

void Renderer::update_animations() {
	remove_null_animated_objects();

	std::vector<FLAnimatedObject *> &animated_objects = get_animated_objects();
	for (unsigned int i = 0; i < animated_objects.size(); ++i) {
		if (animated_objects[i] != nullptr)
			animated_objects[i]->update_animation();
	}
}

void Renderer::add_to_world(FLTexturedObject *obj) {
	add_to_world(obj, false);
}

void Renderer::add_custom(FLRenderable *obj) {
	custom_renderables.push_back(obj);
}

void Renderer::remove_custom(FLRenderable *obj) {
	custom_renderables.erase(
			std::remove(custom_renderables.begin(), 
					custom_renderables.end(), obj),
			custom_renderables.end());
}

void Renderer::add_to_world(FLTexturedObject *obj, bool scenery) {
	if (scenery) {
		scenery_surface->add_object(obj);
	} else {
		world_surface->add_object(obj);
	}
}

void Renderer::remove_from_world(FLTexturedObject *obj) {
	world_surface->remove_object(obj);
	scenery_surface->remove_object(obj);
}

void Renderer::clear() {
	world_surface->clear();
}

unsigned int Renderer::get_screen_width() { return screen_width; }

unsigned int Renderer::get_screen_height() { return screen_height; }

FLFramebufferShader *Renderer::get_water_shader() {
	return &water_shader;
}

FLCamera* Renderer::get_world_camera() {
	return &world_camera;
}

