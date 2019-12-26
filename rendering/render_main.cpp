/*
 * 	render_main.cpp
 *
 * 	Main rendering functionality.
 *
 */

#include <fstream>
#include <glm/ext.hpp>

#include "../basic_types/basic_types.h"
#include "../logging/logging.h"
#include "fl_rect_shader.h"
#include "rendering.h"

#define DEFAULT_PROGRAM 0

void Renderer::render() {
	glClear( GL_COLOR_BUFFER_BIT );

	if ( !cur_shader ) {
		log_warning( "No shader for rendering." );
		return;
	}

	FLRectShader* shader = (FLRectShader*) cur_shader;
	shader->set_modelview(glm::mat4(1.0));
	shader->update_modelview();

	shader->enable_vertex_pointer();

		glBindBuffer( GL_ARRAY_BUFFER, gVBO );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );

		shader->set_vertex_pointer( 0, NULL );
		glDrawElements( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL );
	
	shader->disable_vertex_pointer();
}

void Renderer::render_and_swap() {
	render();
	SDL_GL_SwapWindow( window );
}

