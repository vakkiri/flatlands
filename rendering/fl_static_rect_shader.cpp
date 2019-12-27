/*
 *	fl_static_rect_shader.cpp
 *
 *
 */

#include "../logging/logging.h"
#include "fl_static_rect_shader.h"

bool FLStaticRectShader::create_program( std::string program_name ) {
    if ( !FLPolygonShader::create_program( program_name ) )
        return false;
    
    return init_poly_buffers();
}

bool FLStaticRectShader::init_poly_buffers() {
	log_progress("Initilizing polygon buffers");
	GLuint rect8_vbo;
	GLuint rect16_vbo;
	GLuint rect32_vbo;
	GLuint rect_ibo;

	// rect buffers vertices
	GLfloat rect8_vertices[] = {
		0.f, 0.f,
		8.f, 0.f,
		8.f, 8.f,
		0.f, 8.f
	};

	GLfloat rect16_vertices[] = {
		0.f, 0.f,
		16.f, 0.f,
		16.f, 16.f,
		0.f, 16.f
	};

	GLfloat rect32_vertices[] = {
		0.f, 0.f,
		32.f, 0.f,
		32.f, 32.f,
		0.f, 32.f
	};

	// rect buffer indices
	GLuint rect_indices[] = {0, 1, 2, 3};

	// generate vertex buffers
	glGenBuffers( 1, &rect8_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, rect8_vbo );
	glBufferData( GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), rect8_vertices, GL_STATIC_DRAW );

	glGenBuffers( 1, &rect16_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, rect16_vbo );
	glBufferData( GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), rect16_vertices, GL_STATIC_DRAW );

	glGenBuffers( 1, &rect32_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, rect32_vbo );
	glBufferData( GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), rect32_vertices, GL_STATIC_DRAW );


	if (glGetError() != GL_NO_ERROR) {
		log_error("Error generating rect vbos");
		return false;
	}

	// generate index buffers
	glGenBuffers( 1, &rect_ibo );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, rect_ibo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLfloat), rect_indices, GL_STATIC_DRAW );

	if (glGetError() != GL_NO_ERROR) {
		log_error("Error generating quad ibo");
		return false;
	}

	// generate vertex arrays
	glGenVertexArrays( 1, &rect8_vao );
	glBindVertexArray( rect8_vao );
	enable_vertex_pointer();
	glBindBuffer( GL_ARRAY_BUFFER, rect8_vbo );
	set_vertex_pointer( 0, NULL );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, rect_ibo );
	glBindVertexArray( 0 );

	glGenVertexArrays( 1, &rect16_vao );
	enable_vertex_pointer();
	glBindVertexArray( rect16_vao );
	glBindBuffer( GL_ARRAY_BUFFER, rect16_vbo );
	set_vertex_pointer( 0, NULL );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, rect_ibo );
	glBindVertexArray( 0 );

	glGenVertexArrays( 1, &rect32_vao );
	enable_vertex_pointer();
	glBindVertexArray( rect32_vao );
	glBindBuffer( GL_ARRAY_BUFFER, rect32_vbo );
	set_vertex_pointer( 0, NULL );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, rect_ibo );
	glBindVertexArray( 0 );

	if (glGetError() != GL_NO_ERROR) {
		log_error("Error generating rect vao");
		return false;
	}

	return true;
}

void FLStaticRectShader::draw_rects8() {
	glBindVertexArray( rect8_vao );
	glDrawElementsInstanced( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL, 1 );
}

void FLStaticRectShader::draw_rects16() {
	glBindVertexArray( rect16_vao );
	glDrawElementsInstanced( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL, 1 );
}

void FLStaticRectShader::draw_rects32() {
	glBindVertexArray( rect32_vao );
	glDrawElementsInstanced( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL, 1 );
}
