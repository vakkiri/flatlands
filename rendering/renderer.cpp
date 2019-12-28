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
#include "rendering.h"
#include "renderable.h"


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
