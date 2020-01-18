/*
 * 	render_main.cpp
 *
 * 	Main rendering functionality.
 *
 */

#include <algorithm>
#include <fstream>
#include <glm/ext.hpp>

#include "../common/basic_types.h"
#include "../logging/logging.h"
#include "animated_object.h"
#include "renderer.h"
#include "renderable.h"
#include "rendered_surface.h"
#include "world_surface.h"


void Renderer::render() {
	clear_null_renderables();

	glClear( GL_COLOR_BUFFER_BIT );

	glBindFramebuffer( GL_FRAMEBUFFER, framebuffer );

	// draw to our frame buffer
	update_animations();
	world_surface->update_buffers();

	// draw background
	textured_rect_shader.set_camera( background_camera );
	textured_rect_shader.update_pc_matrix();

	for ( FLRenderable *r : background_renderables )
		r->render();

	// draw world
	textured_rect_shader.set_camera( world_camera );
	textured_rect_shader.update_pc_matrix();

	for ( FLRenderable *r : world_renderables )
		r->render();

	// draw ui
	// (not implemented yet)

	// render framebuffer to screen	
	textured_rect_shader.set_camera( framebuffer_camera );
	textured_rect_shader.update_pc_matrix();

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	framebuffer_surface->render();
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

FLTexturedRectShader& Renderer::get_textured_rect_shader() {
	return textured_rect_shader;
}

FLWorldSurface* Renderer::get_world_surface() {
	return world_surface;
}

FLTexturedSurface* Renderer::get_tilemap_surface() {
	return tilemap_surface;
}

void Renderer::add_animated_object( FLAnimatedObject* object ) {
	animated_objects.push_back( object );
}

void Renderer::remove_animated_object( FLAnimatedObject* object ) {
	for ( int i = 0; i < animated_objects.size(); ++i ) {
		if ( animated_objects[i] == object )
			animated_objects[i] = nullptr;
	}
}

void Renderer::remove_null_objects() {
	animated_objects.erase( 
			std::remove_if( 
				animated_objects.begin(), 
				animated_objects.end(), 
				[](FLAnimatedObject* obj) { return obj == nullptr; }), 
			animated_objects.end() );
}

void Renderer::update_animations() {
	for ( int i = 0; i < animated_objects.size(); ++i )
		animated_objects[i]->update_animation();

	remove_null_objects();
}

void Renderer::add_to_world( FLTexturedObject* obj ) {
	world_surface->add_object( obj );
}

void Renderer::remove_from_world( FLTexturedObject* obj ) {
	world_surface->remove_object( obj );
}

void Renderer::clear() {
	clear_world();
	// TODO: clearly we need to separate animated objects which are
	// "inside" the world (ie level-specific) from "outside" the world
	// if we are going to be clearing them like this.
	animated_objects.clear();
}

void Renderer::clear_world() {
	world_surface->clear();
}

