/*
 * 	fl_texture_surface.cpp
 *
 */

#include <iostream>

#include "logging/logging.h"
#include "resources/fl_resources.h"
#include "fl_texture_surface.h"
#include "rendering/renderer.h"
#include "rendering/fl_camera.h"

FLTextureSurface::FLTextureSurface() : persistent(false) {}

FLTextureSurface::FLTextureSurface(texture tex) : 
	camera(nullptr),
	persistent(false),
	tex(tex) {}

FLTextureSurface::FLTextureSurface(bool persistent, std::string tex) : 
	camera(nullptr),
	persistent(persistent),
	tex(*FLResources::getInstance().get_image(tex)) {}

FLTextureSurface::FLTextureSurface(bool persistent, texture tex) :
	camera(nullptr),
	persistent(persistent),
	tex(tex) {}

void FLTextureSurface::push(FLTexture *element) {
	elements.push_back(element);
	dirty = true;
}

void FLTextureSurface::render() {
	FLTexturedRectShader &shader = FLRenderer::get_texture_shader();

	if (dirty) {
		update_buffers();
	}

	shader.bind();

	if (camera) {
		shader.set_camera(camera->mat());
		shader.update_pc_matrix();
	} else {
		shader.set_camera(glm::mat4(1.0));
		shader.update_pc_matrix();
	}

	glBindTexture(GL_TEXTURE_2D, tex.id);
	shader.render(vao, rendered_indices);
}

void FLTextureSurface::set_camera(FLCamera *camera) {
	this->camera = camera;
}

void FLTextureSurface::update_buffers() {
	FLTexturedRectShader &shader = FLRenderer::get_texture_shader();

	int vert_size = 4;	// x, y, s, t
	int num_verts = elements.size() * 4;
	int num_indices = elements.size() * 5;

	float *vbuf = new float[num_verts * vert_size];
	unsigned int *ibuf = new unsigned int[num_indices];

	rendered_indices = num_indices;

	float tex_left;
	float tex_right;
	float tex_top;
	float tex_bot;

	unsigned int i = 0;
	unsigned int step = vert_size * 4; // 4 verts per quad

	for (auto element : elements) {
		float x = element->shape->x();
		float y = element->shape->y();
		float w = element->shape->w();
		float h = element->shape->h();
		float s = element->s;
		float t = element->t;

		tex_top = t / tex.h;
		tex_bot = tex_top + h / tex.h;

		if (!element->reversed) {
			tex_left = s / tex.w;
			tex_right = tex_left + w / tex.w;
		} else {
			tex_right = s / tex.w;
			tex_left = tex_left + w / tex.w;
		}

                // vertex position
                vbuf[(i * step)] = x;
                vbuf[(i * step) + 1] = y;
                vbuf[(i * step) + 4] = x + w;
                vbuf[(i * step) + 5] = y;
                vbuf[(i * step) + 8] = x + w;
                vbuf[(i * step) + 9] = y + h;
                vbuf[(i * step) + 12] = x;
                vbuf[(i * step) + 13] = y + h;

                // vertex texture position
                vbuf[(i * step) + 2] = tex_left;
                vbuf[(i * step) + 3] = tex_top;
                vbuf[(i * step) + 6] = tex_right;
                vbuf[(i * step) + 7] = tex_top;
                vbuf[(i * step) + 10] = tex_right;
                vbuf[(i * step) + 11] = tex_bot;
                vbuf[(i * step) + 14] = tex_left;
                vbuf[(i * step) + 15] = tex_bot;

                // indicies
                ibuf[(i * 5)] = i * 4;
                ibuf[(i * 5) + 1] = (i * 4) + 1;
                ibuf[(i * 5) + 2] = (i * 4) + 2;
                ibuf[(i * 5) + 3] = (i * 4) + 3;
                ibuf[(i * 5) + 4] = RESTART;

		++i;
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

        shader.enable_vertex_pointer();
        shader.enable_tex_coord_pointer();

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        shader.set_vertex_pointer(4 * sizeof(float), NULL);
        shader.set_tex_coord_pointer(
		4 * sizeof(float),
		(const void *)(2 * sizeof(float))
	);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBindVertexArray(0);

        delete [] vbuf;
        delete [] ibuf;

	if (!persistent) {
		elements.clear();
	}

	dirty = false;
}

void FLTextureSurface::set_tex(texture tex) {
	this->tex = tex;
}

void FLTextureSurface::set_tex(std::string name) {
	this->tex = *FLResources::getInstance().get_image(name);
}

