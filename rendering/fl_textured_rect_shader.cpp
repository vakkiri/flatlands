/*
 * 	fl_textured_rect_shader.cpp
 *
 */

#include "../logging/logging.h"
#include "../resources/fl_resources.h"
#include "../tilemap/tile.h"
#include "fl_textured_rect_shader.h"
#include "texture.h"

#define RESTART 0xFFFF

FLTexturedRectShader::FLTexturedRectShader() : FLStaticRectShader() {};

FLTexturedRectShader::FLTexturedRectShader( std::string program_name ) : FLStaticRectShader() {
	create_program( program_name );
	bind();
	set_tex_unit(0);
}

bool FLTexturedRectShader::create_program( std::string program_name ) {
	log_progress( "FLTexturedRectShader::create_program");

	if ( !FLStaticRectShader::create_program( program_name ) )
		return false;

	glGenBuffers( 1, &tex_vbo );

	if ( glGetError() != GL_NO_ERROR ) {
		log_error( "Could not generate texture vbo" );
		return false;
	}

	intex_coord_location = glGetAttribLocation( program_id, "inTexCoord" );

	if ( intex_coord_location == -1 ) {
		log_error( "Could not find inTexCoord in shader" );
		return false;
	}

	tex_unit_location = glGetUniformLocation( program_id, "texUnit" );

	if ( tex_unit_location == -1 ) {
		log_error( "Could not find inTexCoord in shader" );
		return false;
	}

	return true;
}

void FLTexturedRectShader::set_geometry( std::vector<tile*>& tiles ) {
	log_progress("Updating texture shader geometry");

	num_verts = tiles.size() * 8;
	num_indices = tiles.size() * 5;

	float *vbuf = new float[num_verts];
	float *tbuf = new float[num_verts];
	unsigned int *ibuf = new unsigned int[num_indices];

	float tleft;
	float tright;
	float ttop;
	float tbot;

	for (int i = 0; i < tiles.size(); i++) {
		tleft = (tiles[i]->u / tex->w);
		tright = tleft + (tiles[i]->bounds.w / tex->w);
		ttop = (tiles[i]->v / tex->h);
		tbot = ttop + (tiles[i]->bounds.h / tex->h);

		vbuf[(i*8)] = tiles[i]->bounds.x;
		tbuf[(i*8)] = tleft;
		vbuf[(i*8)+1] = tiles[i]->bounds.y;
		tbuf[(i*8)+1] = ttop;

		vbuf[(i*8)+2] = tiles[i]->bounds.x + tiles[i]->bounds.w;
		tbuf[(i*8)+2] = tright;
		vbuf[(i*8)+3] = tiles[i]->bounds.y;
		tbuf[(i*8)+3] = ttop;

		vbuf[(i*8)+4] = tiles[i]->bounds.x + tiles[i]->bounds.w;
		tbuf[(i*8)+4] = tright;
		vbuf[(i*8)+5] = tiles[i]->bounds.y + tiles[i]->bounds.h;
		tbuf[(i*8)+5] = tbot;

		vbuf[(i*8)+6] = tiles[i]->bounds.x;
		tbuf[(i*8)+6] = tleft;
		vbuf[(i*8)+7] = tiles[i]->bounds.y + tiles[i]->bounds.h;
		tbuf[(i*8)+7] = tbot;

		ibuf[(i*5)] = (i * 4);
		ibuf[(i*5)+1] = (i * 4)+1;
		ibuf[(i*5)+2] = (i * 4)+2;
		ibuf[(i*5)+3] = (i * 4)+3;
		ibuf[(i*5)+4] = RESTART;
	}

	glBindBuffer( GL_ARRAY_BUFFER, rect_vbo );
	glBufferData( GL_ARRAY_BUFFER, num_verts * sizeof(float), vbuf, GL_STATIC_DRAW);

	if ( glGetError() != GL_NO_ERROR )
		log_error( "Error buffering vertex vbo" );

	glBindBuffer( GL_ARRAY_BUFFER, tex_vbo );
	glBufferData( GL_ARRAY_BUFFER, num_verts * sizeof(float), tbuf, GL_STATIC_DRAW);

	if ( glGetError() != GL_NO_ERROR )
		log_error( "Error buffering texture vbo" );


	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, rect_ibo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(unsigned int), ibuf, GL_STATIC_DRAW );

	if ( glGetError() != GL_NO_ERROR )
		log_error( "Error buffering vbo" );

	glBindVertexArray( rect_vao );
	glPrimitiveRestartIndex( RESTART );

	// enable attrib pointers
	enable_vertex_pointer();
	glEnableVertexAttribArray( intex_coord_location );

	glBindBuffer( GL_ARRAY_BUFFER, rect_vbo );
	set_vertex_pointer( 0, NULL );

	glBindBuffer( GL_ARRAY_BUFFER, tex_vbo );
	set_tex_coord_pointer( 0, NULL );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, rect_ibo );
	glBindVertexArray( 0 );

	delete [] vbuf;
	delete [] tbuf;
	delete [] ibuf;
}

void FLTexturedRectShader::set_tex_coord_pointer( GLsizei stride, const GLvoid* data ) {
	glVertexAttribPointer( intex_coord_location, 2, GL_FLOAT, GL_FALSE, stride, data );
}

void FLTexturedRectShader::set_tex_unit( GLuint unit ) {
	glUniform1i( tex_unit_location, unit );
}

void FLTexturedRectShader::set_tex( texture *tex ) {
	this->tex = tex;
}

void FLTexturedRectShader::set_tex( std::string name ) {
	tex = FLResources::getInstance().get_image( name );
}

void FLTexturedRectShader::draw_rects() {
	glBindTexture( GL_TEXTURE_2D, tex->id );
	FLStaticRectShader::draw_rects();
}
