/*
 * 	rendered_surface.cpp
 *
 */

#include "../logging/logging.h"
#include "../resources/fl_resources.h"
#include "textured_object.h"
#include "fl_textured_rect_shader.h"
#include "rendered_surface.h"

#define RESTART 0xFFFF

FLRenderedSurface::FLRenderedSurface() {
	// initialize buffers
	glGenBuffers( 1, &vbo );
	glGenBuffers( 1, &ibo );
	glGenBuffers( 1, &vao );

	if ( glGetError() != GL_NO_ERROR )
		log_error( "Could not initialize surface buffers" );


	// initially we have no geometry
	num_verts = 0;
	num_indices = 0;
}

FLTexturedSurface::FLTexturedSurface( FLTexturedRectShader& s ) : shader(s), FLRenderedSurface() {
	tex = nullptr;
}

void FLTexturedSurface::set_tex( texture *tex ) {
	this->tex = tex;
}

void FLTexturedSurface::set_tex( std::string name ) {
	tex = FLResources::getInstance().get_image( name );
}

void FLTexturedSurface::update_buffers( std::vector<FLTexturedObject*>& objects ) {
	log_progress( "Updating surface buffers" );

	unsigned int vert_size = 4;	// location x, location y, tex x, tex y
	num_verts = objects.size() * 4;
	num_indices = objects.size() * 5;	// 4 verts + RESTART
	unsigned int step = vert_size * 4; 	// vert_size * 4 verts per quad

	float *vbuf = new float[num_verts * vert_size];	// position verts + tex verts
	unsigned int *ibuf = new unsigned int[num_indices];

	float tleft;
	float tright;
	float ttop;
	float tbot;

	for ( int i = 0; i < objects.size(); i++ ) {
		tleft = ( objects[i]->s() / tex->w );
		tright = tleft + ( objects[i]->w() / tex->w );
		ttop = ( objects[i]->t() / tex->h );
		tbot = ttop + ( objects[i]->h() / tex->h );

		// vertex position
		vbuf[(i*step)] = objects[i]->x();
		vbuf[(i*step)+1] = objects[i]->y();
		vbuf[(i*step)+4] = objects[i]->x() + objects[i]->w();
		vbuf[(i*step)+5] = objects[i]->y();
		vbuf[(i*step)+8] = objects[i]->x() + objects[i]->w();
		vbuf[(i*step)+9] = objects[i]->y() + objects[i]->h();
		vbuf[(i*step)+12] = objects[i]->x();
		vbuf[(i*step)+13] = objects[i]->y() + objects[i]->h();

		// vertex texture position
		vbuf[(i*step)+2] = tleft;
		vbuf[(i*step)+3] = ttop;
		vbuf[(i*step)+6] = tright;
		vbuf[(i*step)+7] = ttop;
		vbuf[(i*step)+10] = tright;
		vbuf[(i*step)+11] = tbot;
		vbuf[(i*step)+14] = tleft;
		vbuf[(i*step)+15] = tbot;

		// indicies
		ibuf[(i*5)] = i * 4;
		ibuf[(i*5)+1] = (i * 4) + 1;
		ibuf[(i*5)+2] = (i * 4) + 2;
		ibuf[(i*5)+3] = (i * 4) + 3;
		ibuf[(i*5)+4] = RESTART;
	}

	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, num_verts * vert_size * sizeof(float), vbuf, GL_STATIC_DRAW );

	if ( glGetError() != GL_NO_ERROR )
		log_error( "Error buffering vbo" );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(unsigned int), ibuf, GL_STATIC_DRAW );

	if ( glGetError() != GL_NO_ERROR )
		log_error( "Error buffering ibo" );

	glBindVertexArray( vao );
	glPrimitiveRestartIndex( RESTART );

	shader.enable_vertex_pointer();
	shader.enable_tex_coord_pointer();

		glBindBuffer( GL_ARRAY_BUFFER, vbo );
		shader.set_vertex_pointer( 4 * sizeof(float), NULL );
		shader.set_tex_coord_pointer( 4 * sizeof(float), (const void *) (2 * sizeof(float)) );

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
		glBindVertexArray( 0 );

	delete [] vbuf;
	delete [] ibuf;
}

void FLTexturedSurface::render() {
	glBindTexture( GL_TEXTURE_2D, tex->id );
	shader.render( vao, num_indices );
}
