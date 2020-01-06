/*
 * 	rendered_surface.cpp
 *
 */

#include "../logging/logging.h"
#include "../resources/fl_resources.h"
#include "../rendering/renderer.h"
#include "textured_object.h"
#include "fl_textured_rect_shader.h"
#include "rendered_surface.h"

#define RESTART 0xFFFF

FLRenderedSurface::FLRenderedSurface() : FLRenderable() {
	// initialize buffers
	vbo = 0;
	ibo = 0;
	vao = 0;
	glGenBuffers( 1, &vbo );
	glGenBuffers( 1, &ibo );
	glGenVertexArrays( 1, &vao );

	if ( vbo == 0 )
		log_error( "Could not generate VBO" );
	if ( ibo == 0 )
		log_error( "Could not generate IBO" );
	if ( vao == 0 )
		log_error( "Could not generate VAO" );
	if ( glGetError() != GL_NO_ERROR )
		log_error( "Could not initialize surface buffers" );

	// initially we have no geometry
	num_verts = 0;
	num_indices = 0;
}

FLTexturedSurface::FLTexturedSurface() : shader(Renderer::getInstance().get_textured_rect_shader()), FLRenderedSurface() {
	tex = nullptr;
}

void FLTexturedSurface::set_tex( texture *tex ) {
	this->tex = tex;
}

void FLTexturedSurface::set_tex( std::string name ) {
	tex = FLResources::getInstance().get_image( name );
}

void FLTexturedSurface::update_buffers( std::vector<FLTexturedObject*>& objects ) {
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

	if ( glGetError() != GL_NO_ERROR )
		log_error( "Error while binding VAO" );

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

void FLTexturedSurface::update_buffers( FLTexturedObject* object ) {
	unsigned int vert_size = 4;		// location x, location y, tex x, tex y
	num_verts = 4;
	num_indices = 5;			// 4 verts + RESTART

	float *vbuf = new float[num_verts * vert_size];	// position verts + tex verts
	unsigned int *ibuf = new unsigned int[num_indices];

	float tleft;
	float tright;
	float ttop;
	float tbot;

	if ( !object->reversed() ) {
		tleft = ( object->s() / tex->w );
		tright = tleft + ( object->w() / tex->w );
		ttop = ( object->t() / tex->h );
		tbot = ttop + ( object->h() / tex->h );
	}
	else {
		tright = ( object->s() / tex->w );
		tleft = tright + ( object->w() / tex->w );
		ttop = ( object->t() / tex->h );
		tbot = ttop + ( object->h() / tex->h );
	}

	// vertex position
	vbuf[0] = object->x();
	vbuf[1] = object->y();
	vbuf[4] = object->x() + object->w();
	vbuf[5] = object->y();
	vbuf[8] = object->x() + object->w();
	vbuf[9] = object->y() + object->h();
	vbuf[12] = object->x();
	vbuf[13] = object->y() + object->h();

	// vertex texture position
	vbuf[2] = tleft;
	vbuf[3] = ttop;
	vbuf[6] = tright;
	vbuf[7] = ttop;
	vbuf[10] = tright;
	vbuf[11] = tbot;
	vbuf[14] = tleft;
	vbuf[15] = tbot;

	// indicies
	ibuf[0] = 0;
	ibuf[1] = 1;
	ibuf[2] = 2;
	ibuf[3] = 3;
	ibuf[4] = RESTART;

	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, num_verts * vert_size * sizeof(float), vbuf, GL_STATIC_DRAW );

	if ( glGetError() != GL_NO_ERROR )
		log_error( "Error buffering vbo for single object" );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(unsigned int), ibuf, GL_STATIC_DRAW );

	if ( glGetError() != GL_NO_ERROR )
		log_error( "Error buffering ibo for single object" );

	glBindVertexArray( vao );

	if ( glGetError() != GL_NO_ERROR )
		log_error( "Error while binding VAO" );

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
