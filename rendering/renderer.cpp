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
#include "renderer.h"
#include "renderable.h"
#include "rendered_surface.h"


void Renderer::render() {
	clear_null_renderables();

	glClear( GL_COLOR_BUFFER_BIT );

	for ( FLRenderable *r : renderables )
		r->render();
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

FLTexturedRectShader& Renderer::get_textured_rect_shader() {
	return textured_rect_shader;
}

FLTexturedSurface* Renderer::get_world_surface() {
	return world_surface;
}

FLTexturedSurface* Renderer::get_tilemap_surface() {
	return tilemap_surface;
}
