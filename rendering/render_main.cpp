/*
 * 	render_main.cpp
 *
 * 	Main rendering functionality.
 *
 */

#include <fstream>

#include "../logging/logging.h"
#include "rendering.h"

#define DEFAULT_PROGRAM 0

void Renderer::render() {
	glClear( GL_COLOR_BUFFER_BIT );

	glUseProgram( gProgramID );
	glEnableVertexAttribArray( gVertexPos2DLocation );

	// set vertex data in VBO
	glBindBuffer( GL_ARRAY_BUFFER, gVBO );
	glVertexAttribPointer( gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

	// set index data in IBO and render
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
	glDrawElements( GL_QUADS, 4, GL_UNSIGNED_INT, NULL );

	glDisableVertexAttribArray( gVertexPos2DLocation );

	glUseProgram( DEFAULT_PROGRAM );
}

void Renderer::render_and_swap() {
	render();
	SDL_GL_SwapWindow( window );
}

GLuint Renderer::attach_shader(const GLchar* source[], GLenum shaderType) {
	if (gProgramID == 0) {
		log_error("Cannot attach shader: program ID is 0");
		return 0;
	}

	GLuint shader = glCreateShader( shaderType );
	glShaderSource( shader, 1, source, NULL );
	glCompileShader( shader );

	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &vShaderCompiled );
	
	if ( vShaderCompiled != GL_TRUE ) {
		log_error("Unable to compile vertex shader");
		return 0;
	}

	glAttachShader( gProgramID, shader );

	return shader;
}


GLuint Renderer::attach_shader(std::string path, GLenum shaderType) {
	GLuint shader = 0;
	std::string shader_string;
	std::ifstream infile( path.c_str() );

	if ( infile ) {
		shader_string.assign( std::istreambuf_iterator<char>( infile ), std::istreambuf_iterator<char>() );
		const GLchar* source = shader_string.c_str();
		shader = attach_shader((const GLchar**)&source, shaderType);
	}
	else
		log_error("Could not open shader from file: " + path);

	// If infile was not loaded properly, this will still be 0
	return shader;
}

