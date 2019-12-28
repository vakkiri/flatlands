/*
 *	fl_static_rect_shader.cpp
 *
 *
 */

#include <iostream>

#include "../logging/logging.h"
#include "fl_static_rect_shader.h"

#define RESTART 0xff

FLStaticRectShader::FLStaticRectShader() : FLPolygonShader() {
	create_program("instanced_rect_shader");
	bind();
}

bool FLStaticRectShader::create_program( std::string program_name ) {
	if ( !FLPolygonShader::create_program( program_name ) )
		return false;

	glGenBuffers( 1, &rect_vbo );
	glGenBuffers( 1, &rect_ibo );
	glGenVertexArrays( 1, &rect_vao );

	if ( glGetError() != GL_NO_ERROR ) {
		log_error("Could not create StaticRectShader");
		return false;
	}

	return true;
}

void FLStaticRectShader::set_geometry(std::vector<rect> &rects) {
	log_progress("Updating static geometry");

	num_verts = rects.size() * 8;
	num_indices = rects.size() * 5;

	float *vbuf = new float[num_verts];
	unsigned int *ibuf = new unsigned int[num_indices];

	for (int i = 0; i < rects.size(); i++) {
		std::cout << i << std::endl;
		vbuf[(i*8)] = rects[i].x;
		vbuf[(i*8)+1] = rects[i].y;
		vbuf[(i*8)+2] = rects[i].x + rects[i].w;
		vbuf[(i*8)+3] = rects[i].y;
		vbuf[(i*8)+4] = rects[i].x + rects[i].w;
		vbuf[(i*8)+5] = rects[i].y + rects[i].h;
		vbuf[(i*8)+6] = rects[i].x;
		vbuf[(i*8)+7] = rects[i].y + rects[i].h;

		ibuf[(i * 5)] = (i * 4);
		ibuf[(i * 5)+1] = (i * 4)+1;
		ibuf[(i * 5)+2] = (i * 4)+2;
		ibuf[(i * 5)+3] = (i * 4)+3;
		ibuf[(i * 5)+4] = RESTART;
	}

	glBindBuffer( GL_ARRAY_BUFFER, rect_vbo );
	glBufferData( GL_ARRAY_BUFFER, num_verts * sizeof(float), vbuf, GL_STATIC_DRAW);

	if ( glGetError() != GL_NO_ERROR )
		log_error("Error buffering vbo");

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, rect_ibo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(unsigned int), ibuf, GL_STATIC_DRAW);

	if ( glGetError() != GL_NO_ERROR )
		log_error("Error buffering ibo");


	glBindVertexArray( rect_vao );
	glPrimitiveRestartIndex( RESTART );
	enable_vertex_pointer();
	glBindBuffer( GL_ARRAY_BUFFER, rect_vbo );
	set_vertex_pointer( 0, NULL );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, rect_ibo );
	glBindVertexArray( 0 );

	delete vbuf;
	delete ibuf;

}

void FLStaticRectShader::draw_rects() {
	glBindVertexArray( rect_vao );
	glDrawElements( GL_TRIANGLE_FAN, num_indices, GL_UNSIGNED_INT, NULL);
}
