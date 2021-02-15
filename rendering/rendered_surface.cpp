/*
 * 	rendered_surface.cpp
 *
 */

#include <iostream>

#include "../logging/logging.h"
#include "../rendering/renderer.h"
#include "../resources/fl_resources.h"
#include "fl_colored_poly_shader.h"
#include "fl_textured_rect_shader.h"
#include "rendered_surface.h"
#include "textured_object.h"

FLRenderedSurface::FLRenderedSurface() : FLRenderable() {
	// initialize buffers
	vbo = 0;
	ibo = 0;
	vao = 0;
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
	glGenVertexArrays(1, &vao);

	if (vbo == 0)
		log_error("Could not generate VBO");
	if (ibo == 0)
		log_error("Could not generate IBO");
	if (vao == 0)
		log_error("Could not generate VAO");
	if (glGetError() != GL_NO_ERROR)
		log_error("Could not initialize surface buffers");

	// initially we have no geometry
	num_verts = 0;
	num_indices = 0;
}

FLColoredSurface::FLColoredSurface() : FLRenderedSurface() { shader = nullptr; }

void FLColoredSurface::render() {
	if (shader != nullptr)
		shader->render(vao, num_indices);
	else
		log_warning("Attempted to render with null shader");
}

void FLColoredSurface::clear_verts() { verts.clear(); }

void FLColoredSurface::update_buffers() {
	if (verts.size() > 0) {
		num_indices = indices.size();

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, verts.size() * 6 * sizeof(float),
					 &(verts[0]), GL_STATIC_DRAW);
		if (glGetError() != GL_NO_ERROR)
			log_error("Error buffering vbo");

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
					 indices.size() * sizeof(unsigned int), &(indices[0]),
					 GL_STATIC_DRAW);

		if (glGetError() != GL_NO_ERROR)
			log_error("Error buffering ibo");

		glBindVertexArray(vao);

		if (glGetError() != GL_NO_ERROR)
			log_error("Error buffering vao");

		glPrimitiveRestartIndex(RESTART);

		shader->enable_vertex_pointer();
		shader->enable_color_pointer();

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		shader->set_vertex_pointer(6 * sizeof(float), NULL);
		shader->set_color_pointer(6 * sizeof(float),
								  (const void *)(2 * sizeof(float)));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBindVertexArray(0);
	} else {
		num_indices = 0;
	}
}

void FLColoredSurface::add_verts(std::vector<fl_colored_vertex> &new_verts) {
	// The "value" of each index for the ibuf should be the position of its
	// associated vertex in our vertex buffer. We use verts.size() to find this
	// position instead of indices.size() because the index vector contains
	// RESTART indices.
	for (unsigned int i = 0; i < new_verts.size(); ++i)
		indices.push_back(verts.size() + i);

	// signify the end of a piece of geometry using the RESTART index
	indices.push_back(RESTART);

	verts.insert(verts.end(), new_verts.begin(), new_verts.end());
}

void FLColoredSurface::set_shader(FLColoredPolyShader *shader) {
	this->shader = shader;
}

FLTexturedSurface::FLTexturedSurface() : FLRenderedSurface() {
	tex = nullptr;
	shader = nullptr;
}

void FLTexturedSurface::set_tex(texture *tex) { this->tex = tex; }

void FLTexturedSurface::set_tex(std::string name) {
	tex = FLResources::getInstance().get_image(name);
}

void FLTexturedSurface::update_buffers(
	std::vector<FLTexturedObject *> &objects) {
	unsigned int vert_size = 4; // location x, location y, tex x, tex y
	num_verts = objects.size() * 4;
	num_indices = objects.size() * 5;  // 4 verts + RESTART
	unsigned int step = vert_size * 4; // vert_size * 4 verts per quad

	float *vbuf =
		new float[num_verts * vert_size]; // position verts + tex verts
	unsigned int *ibuf = new unsigned int[num_indices];

	float tleft;
	float tright;
	float ttop;
	float tbot;

	for (unsigned int i = 0; i < objects.size(); i++) {
		if (!objects[i]->is_visible()) {
			tleft = 0.f;
			tright = 0.f;
			ttop = 0.f;
			tbot = 0.f;
		} else if (!objects[i]->reversed()) {
			tleft = (objects[i]->s() / tex->w);
			tright = tleft + (objects[i]->w() / tex->w);
			ttop = (objects[i]->t() / tex->h);
			tbot = ttop + (objects[i]->h() / tex->h);
		} else {
			tright = (objects[i]->s() / tex->w);
			tleft = tright + (objects[i]->w() / tex->w);
			ttop = (objects[i]->t() / tex->h);
			tbot = ttop + (objects[i]->h() / tex->h);
		}

		// vertex position
		vbuf[(i * step)] = objects[i]->x();
		vbuf[(i * step) + 1] = objects[i]->y();
		vbuf[(i * step) + 4] = objects[i]->x() + objects[i]->w();
		vbuf[(i * step) + 5] = objects[i]->y();
		vbuf[(i * step) + 8] = objects[i]->x() + objects[i]->w();
		vbuf[(i * step) + 9] = objects[i]->y() + objects[i]->h();
		vbuf[(i * step) + 12] = objects[i]->x();
		vbuf[(i * step) + 13] = objects[i]->y() + objects[i]->h();

		// vertex texture position
		vbuf[(i * step) + 2] = tleft;
		vbuf[(i * step) + 3] = ttop;
		vbuf[(i * step) + 6] = tright;
		vbuf[(i * step) + 7] = ttop;
		vbuf[(i * step) + 10] = tright;
		vbuf[(i * step) + 11] = tbot;
		vbuf[(i * step) + 14] = tleft;
		vbuf[(i * step) + 15] = tbot;

		// indicies
		ibuf[(i * 5)] = i * 4;
		ibuf[(i * 5) + 1] = (i * 4) + 1;
		ibuf[(i * 5) + 2] = (i * 4) + 2;
		ibuf[(i * 5) + 3] = (i * 4) + 3;
		ibuf[(i * 5) + 4] = RESTART;
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, num_verts * vert_size * sizeof(float), vbuf,
				 GL_STATIC_DRAW);

	if (glGetError() != GL_NO_ERROR)
		log_error("Error buffering vbo");

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(unsigned int),
				 ibuf, GL_STATIC_DRAW);

	if (glGetError() != GL_NO_ERROR)
		log_error("Error buffering ibo");

	glBindVertexArray(vao);

	if (glGetError() != GL_NO_ERROR)
		log_error("Error while binding VAO");

	glPrimitiveRestartIndex(RESTART);

	shader->enable_vertex_pointer();
	shader->enable_tex_coord_pointer();

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	shader->set_vertex_pointer(4 * sizeof(float), NULL);
	shader->set_tex_coord_pointer(4 * sizeof(float),
								  (const void *)(2 * sizeof(float)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBindVertexArray(0);

	delete [] vbuf;
	delete [] ibuf;
}

void FLTexturedSurface::update_buffers(FLTexturedObject *object) {
	unsigned int vert_size = 4; // location x, location y, tex x, tex y
	num_verts = 4;
	num_indices = 5; // 4 verts + RESTART

	float *vbuf =
		new float[num_verts * vert_size]; // position verts + tex verts
	unsigned int *ibuf = new unsigned int[num_indices];

	float tleft;
	float tright;
	float ttop;
	float tbot;

	if (!object->reversed()) {
		tleft = (object->s() / tex->w);
		tright = tleft + (object->w() / tex->w);
		ttop = (object->t() / tex->h);
		tbot = ttop + (object->h() / tex->h);
	} else {
		tright = (object->s() / tex->w);
		tleft = tright + (object->w() / tex->w);
		ttop = (object->t() / tex->h);
		tbot = ttop + (object->h() / tex->h);
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

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, num_verts * vert_size * sizeof(float), vbuf,
				 GL_STATIC_DRAW);

	if (glGetError() != GL_NO_ERROR)
		log_error("Error buffering vbo for single object");

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(unsigned int),
				 ibuf, GL_STATIC_DRAW);

	if (glGetError() != GL_NO_ERROR)
		log_error("Error buffering ibo for single object");

	glBindVertexArray(vao);

	if (glGetError() != GL_NO_ERROR)
		log_error("Error while binding VAO");

	glPrimitiveRestartIndex(RESTART);

	shader->enable_vertex_pointer();
	shader->enable_tex_coord_pointer();

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	shader->set_vertex_pointer(4 * sizeof(float), NULL);
	shader->set_tex_coord_pointer(4 * sizeof(float),
								  (const void *)(2 * sizeof(float)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBindVertexArray(0);

	delete[] vbuf;
	delete[] ibuf;
}

void FLTexturedSurface::render() {
	if (shader != nullptr && tex != nullptr) {
		glBindTexture(GL_TEXTURE_2D, tex->id);
		shader->render(vao, num_indices);
	} else {
		log_warning("Attempted to render with null shader or texture.");
	}
}

void FLTexturedSurface::set_shader(FLTexturedRectShader *shader) {
	this->shader = shader;
}
