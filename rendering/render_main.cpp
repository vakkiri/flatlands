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

void Renderer::render() {
	glClear( GL_COLOR_BUFFER_BIT );

	static_rect_shader.draw_rects();
}

void Renderer::render_and_swap() {
	render();
	SDL_GL_SwapWindow( window );
}

