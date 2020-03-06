/*
 * 	render_main.cpp
 *
 * 	Main rendering functionality.
 *
 */

#include <iostream>
#include <fstream>
#include <glm/ext.hpp>

#include "../common/basic_types.h"
#include "../custom/angel.h"
#include "../resources/fl_resources.h"
#include "../ui/fl_ui_manager.h"
#include "../world/world_environment.h"
#include "../logging/logging.h"
#include "animated_object.h"
#include "renderer.h"
#include "renderable.h"
#include "rendered_surface.h"
#include "world_surface.h"
#include "fl_distortion_surface.h"


void Renderer::flip_framebuffer() {
	if ( current_rendered_texture == &alt_rendered_texture )
		current_rendered_texture = &main_rendered_texture;
	else
		current_rendered_texture = &alt_rendered_texture;

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *current_rendered_texture, 0 );
}

texture* Renderer::screen_texture() {
	// The texture returned will be the texture we are reading from, which
	// must be the opposite of the texture we are writing to.
	if ( current_rendered_texture == &alt_rendered_texture )
		return framebuffer_texture;
	else
		return alt_framebuffer_texture;
}

void Renderer::render_to_screen() {
	// Make sure we get any changes to the framebuffer
	flip_framebuffer();

	// Bind our shader and ensure we are drawing to the screen
	framebuffer_shader.bind();
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	framebuffer_surface->set_tex( screen_texture() );

	// Render the screen quad
	framebuffer_surface->render();
}

void Renderer::prepare_to_render() {
	clear_null_renderables();

	glClear( GL_COLOR_BUFFER_BIT );

	glBindFramebuffer( GL_FRAMEBUFFER, framebuffer );

	current_rendered_texture = &main_rendered_texture;
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, main_rendered_texture, 0 );

}

void Renderer::render() {

	prepare_to_render();

	// TODO: animation updating should be a separate component from rendering
	update_animations();

	// TODO: updating surfaces should be separate from rendering or its own
	// method
	world_surface->update_buffers();

	// draw background to framebuffer 
	textured_rect_shader.bind();
	textured_rect_shader.set_camera( background_camera );
	textured_rect_shader.update_pc_matrix();

	background_surface->set_tex( FLResources::getInstance().get_image("background") );
	background_surface->set_shader( &textured_rect_shader );
	background_surface->render();

	// draw particle system over background
	flip_framebuffer();
	background_shader.bind();
	background_distortion_surface->render();

	// now distort the framebuffer
	flip_framebuffer();
	wave_shader.bind();
	background_surface->set_shader( &wave_shader );
	background_surface->set_tex( screen_texture() );
	background_surface->render();

	// draw world -----------------------------------------
	textured_rect_shader.bind();
	textured_rect_shader.set_camera( world_camera );
	textured_rect_shader.update_pc_matrix();

	for ( FLRenderable *r : world_renderables )
		r->render();

	// draw custom effects
	// custom effects are those which are rendered with a custom shader
	// instead of the standard textured_rect_shader

	// render framebuffer to screen	
	render_to_screen();

	// UI
	// maybe this should come before render_to_screen?
	FLUIManager::getInstance().render();
}

void Renderer::render_and_swap() {
	render();
	SDL_GL_SwapWindow( window );
}

void Renderer::clear_null_renderables() {
	std::vector<int> nulls;

	for ( int i = 0; i < world_renderables.size(); i++ ) {
		if ( world_renderables[i] == nullptr )
			nulls.push_back(i);
	}

	for ( int i : nulls ) {
		world_renderables[i] = world_renderables.back();
		world_renderables.pop_back();
	}
}

void Renderer::translate_world_camera( glm::vec3 translation ) {
	world_camera = glm::translate( world_camera, translation );
}

float Renderer::world_camera_x() { 
	return world_camera[3][0] - (screen_width / 2);
}

float Renderer::world_camera_y() { 
	return world_camera[3][1] - (screen_height / 2);
}

FLTexturedRectShader* Renderer::get_textured_rect_shader() {
	return &textured_rect_shader;
}

FLColoredPolyShader* Renderer::get_colored_poly_shader() {
	return &colored_poly_shader;
}

FLWorldSurface* Renderer::get_world_surface() {
	return world_surface;
}

FLTexturedSurface* Renderer::get_tilemap_surface() {
	return tilemap_surface;
}

void Renderer::update_animations() {
	remove_null_animated_objects();

	std::vector<FLAnimatedObject*>& animated_objects = get_animated_objects();
	for ( int i = 0; i < animated_objects.size(); ++i ) {
		if ( animated_objects[i] != nullptr )
			animated_objects[i]->update_animation();
	}
}

void Renderer::add_to_world( FLTexturedObject* obj ) {
	world_surface->add_object( obj );
}

void Renderer::remove_from_world( FLTexturedObject* obj ) {
	world_surface->remove_object( obj );
}

void Renderer::clear() {
	world_surface->clear();
}

unsigned int Renderer::get_screen_width() {
	return screen_width;
}

unsigned int Renderer::get_screen_height() {
	return screen_height;
}

