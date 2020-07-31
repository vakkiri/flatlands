/*
 * 	fl_text_surface.cpp
 *
 */

#include <iostream>

#include "fl_text_surface.h"
#include "fl_text_shader.h"
#include "texture.h"
#include "text/fl_font.h"
#include "../logging/logging.h"

#define RESTART 0xFFFF

FLTextSurface::FLTextSurface() : FLTexturedSurface() {
	characters.reserve(100);
}

void FLTextSurface::render() {
	update_buffers();

	if (shader != nullptr) {
		glBindTexture(GL_TEXTURE_2D, font->get_texture()->id);
		shader->render(vao, num_indices);
	} else {
		log_warning("Attempted to render with null shader.");
	}

	characters.clear();
}

void FLTextSurface::add_character(fl_character& c) {
	characters.push_back(c);
}

void FLTextSurface::add_character(float x, float y, char c) {
	point st = font->get_character(c);
	fl_color color = {245.f/255.f, 255.f/255.f, 232.f/255.f, 1.f};
	characters.push_back(fl_character{x, y, st.x, st.y, color});
}

void FLTextSurface::update_buffers() {
	unsigned int num_verts = characters.size() * 4;
	num_indices = characters.size() * 5;			// 4 + primitive restart
	unsigned int vert_size = 8;				// x, y, s, t, r, g, b, a
	
	std::vector<float> verts;
	std::vector<unsigned int> indices;

	unsigned int index = 0;
	for (unsigned int i = 1; i <= num_indices; i++) {
		if (i % 5 == 0) {
			indices.push_back(RESTART);
		} else {
			indices.push_back(index++);
		}
	}

	for (auto c : characters) {
		texture* tex = font->get_texture();
		float dx = font->get_char_w();
		float dy = font->get_char_h();
		float ds = dx / tex->w;
		float dt = dy / tex->h;
		float s = c.s / tex->w;
		float t = c.t / tex->h;

		verts.push_back(c.x);
		verts.push_back(c.y);
		verts.push_back(s);
		verts.push_back(t);
		verts.push_back(c.color.r);
		verts.push_back(c.color.g);
		verts.push_back(c.color.b);
		verts.push_back(c.color.a);

		verts.push_back(c.x + dx);
		verts.push_back(c.y);
		verts.push_back(s + ds);
		verts.push_back(t);
		verts.push_back(c.color.r);
		verts.push_back(c.color.g);
		verts.push_back(c.color.b);
		verts.push_back(c.color.a);

		verts.push_back(c.x + dx);
		verts.push_back(c.y + dy);
		verts.push_back(s + ds);
		verts.push_back(t + dt);
		verts.push_back(c.color.r);
		verts.push_back(c.color.g);
		verts.push_back(c.color.b);
		verts.push_back(c.color.a);

		verts.push_back(c.x);
		verts.push_back(c.y + dy);
		verts.push_back(s);
		verts.push_back(t + dt);
		verts.push_back(c.color.r);
		verts.push_back(c.color.g);
		verts.push_back(c.color.b);
		verts.push_back(c.color.a);
	}


	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, num_verts * vert_size * sizeof(float), verts.data(), 
			GL_STATIC_DRAW);

	if (glGetError() != GL_NO_ERROR) {
		log_error("Error buffering vbo");
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(unsigned int), indices.data(), 
			GL_STATIC_DRAW);

	if (glGetError() != GL_NO_ERROR) {
		log_error("Error buffering ibo");
	}

	glBindVertexArray(vao);

	if (glGetError() != GL_NO_ERROR) {
		log_error("Error buffering vao");
	}

	glPrimitiveRestartIndex(RESTART);

	shader->enable_vertex_pointer();
	shader->enable_tex_coord_pointer();
	shader->enable_color_pointer();

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	shader->set_vertex_pointer(8 * sizeof(float), NULL);
	shader->set_tex_coord_pointer(8 * sizeof(float), (const void*)(2 * sizeof(float)));
	shader->set_color_pointer(8 * sizeof(float), (const void*)(4 * sizeof(float)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBindVertexArray(0);

}

void FLTextSurface::set_font(FLBitmapFont *font) {
	this->font = font;
}

void FLTextSurface::set_font(std::string font_name) {
	set_font(get_font(font_name));
}

void FLTextSurface::set_shader(FLTextShader *shader) {
	this->shader = shader;
}
