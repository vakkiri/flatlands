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


void Renderer::render() {
	// TODO: there are some calls in this method which can be moved
	// into init as they aren't necessary every frame
	// Also, a lot of this really should be abstracted into different
	// methods that take a set of objects to render, and a shader to
	// render them with.
	clear_null_renderables();

	// update drawn world
	update_animations();
	world_surface->update_buffers();

	// prepare for rendering
	glClear( GL_COLOR_BUFFER_BIT );

	glBindFramebuffer( GL_FRAMEBUFFER, framebuffer );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, main_rendered_texture, 0 );

	// draw background to framebuffer ----------------------------------
	textured_rect_shader.bind();
	textured_rect_shader.set_camera( background_camera );
	textured_rect_shader.update_pc_matrix();

	background_surface->set_tex( FLResources::getInstance().get_image("background") );
	background_surface->set_shader( &textured_rect_shader );
	background_surface->render();

	// apply shader to background -------------------------
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, alt_rendered_texture, 0 );

	// draw background *again* over new framebuffer, so that we have
	// one copy to read from and one to write over
	for ( FLRenderable *r : background_renderables )
		r->render();

	background_shader.bind();

	background_distortion_surface->render();

	// now apply effects to the result
	wave_shader.bind();
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, main_rendered_texture, 0 );
	background_surface->set_shader( &wave_shader );
	background_surface->set_tex( alt_framebuffer_texture );
	background_surface->render();

	// draw world -----------------------------------------
	textured_rect_shader.bind();
	textured_rect_shader.set_camera( world_camera );
	textured_rect_shader.update_pc_matrix();

	for ( FLRenderable *r : world_renderables )
		r->render();

	// render framebuffer to screen	
	framebuffer_shader.bind();

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	framebuffer_surface->set_tex( framebuffer_texture );
	framebuffer_surface->render();

	// Shader effects for angels
	/*
	custom_shader.bind();
	std::vector<NVAngel*>* angels = FLWorldEnvironment::getInstance().get_angels();	

	if (!angels->empty()) {
		custom_shader.set_dx( (angels->at(0)->x() * 2.f) + (world_camera[3][0]));
		custom_shader.set_dy( (angels->at(0)->y() * 2.f) + (world_camera[3][1]));
	}
	else {
		custom_shader.set_dx( 10000000.f);
		custom_shader.set_dy( 10000000.f );
	}

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	framebuffer_surface->set_tex( framebuffer_texture );
	framebuffer_surface->set_shader( &custom_shader );
	framebuffer_surface->render();
	*/

	// UI

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

