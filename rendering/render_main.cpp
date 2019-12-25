/*
 * 	render_main.cpp
 *
 * 	Main rendering functionality.
 *
 */

#include <fstream>

#include "../logging/logging.h"
#include "fl_shader.h"
#include "rendering.h"

#define DEFAULT_PROGRAM 0

void Renderer::render() {
	glClear( GL_COLOR_BUFFER_BIT );
	glLoadIdentity();

	if ( cur_shader )
		glUseProgram( cur_shader->get_program() );
	else
		log_warning( "No shader for rendering." );

	glEnableClientState( GL_VERTEX_ARRAY );

		// set vertex data in VBO
		glBindBuffer( GL_ARRAY_BUFFER, gVBO );
		glVertexPointer(2, GL_FLOAT, 0, NULL);

		// set index data in IBO and render
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
		glDrawElements( GL_QUADS, 4, GL_UNSIGNED_INT, NULL );

	glDisableClientState( GL_VERTEX_ARRAY );

	glUseProgram( DEFAULT_PROGRAM );
}

void Renderer::render_and_swap() {
	render();
	SDL_GL_SwapWindow( window );
}

