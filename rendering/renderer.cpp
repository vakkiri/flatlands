/*
 * 	render_main.cpp
 *
 * 	Main rendering functionality.
 *
 */

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

	update_animations();
	world_surface->update_buffers();

	// TODO: separate renderables based on used camera
	// ie. world renderables, ui renderables...
	
	// draw world
	textured_rect_shader.set_camera( world_camera );
	textured_rect_shader.update_pc_matrix();

	for ( FLRenderable *r : renderables )
		r->render();

	// draw ui
	// (not implemented yet)
}

void Renderer::render_and_swap() {
	render();
	SDL_GL_SwapWindow( window );
}

void Renderer::clear_null_renderables() {
	std::vector<int> nulls;

	for ( int i = 0; i < renderables.size(); i++ ) {
		if ( renderables[i] == nullptr )
			nulls.push_back(i);
	}

	for ( int i : nulls ) {
		renderables[i] = renderables.back();
		renderables.pop_back();
	}
}

void Renderer::add_renderable(FLRenderable* r) {
	renderables.push_back(r);
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
	// Not implemented
}

void Renderer::update_animations() {
	for ( FLAnimatedObject* object : animated_objects )
		object->update_animation();
}


