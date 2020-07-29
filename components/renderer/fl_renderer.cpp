/*
 * 	fl_renderer.cpp
 *
 */

#include "fl_renderer.h"
#include "fl_renderer_surface.h"

FLRenderer::FLRenderer() {
	kill();
}

void FLRenderer::kill() {
	alive = false;
	shader = nullptr;
	surface = nullptr;
	frame.tex = nullptr;
}

bool FLRenderer::is_alive() {
	return alive;
}

void FLRenderer::set_texture(texture *tex) {
	frame.tex = tex;
}

void FLRenderer::set_st(float s, float t) {
	frame.s = s;
	frame.t = t;
}

void FLRenderer::set_size(float w, float h) {
	frame.w = w;
	frame.h = h;
}

void FLRenderer::set_layer(int layer) {
	this->layer = layer;
}

void FLRenderer::set_dynamic(bool dynamic) {
	this->dynamic = dynamic;
}

void FLRenderer::set_surface(FLRendererSurface *surface) {
	this->surface = surface;
}

void FLRenderer::set_shader(FLShader *shader) {
	this->shader = shader;
}

void FLRenderer::enqueue() {
	surface->push(this);
}

void FLRenderer::render() {
}

int FLRenderer::get_layer() const {
	return layer;
}

bool FLRenderer::is_dynamic() const {
	return dynamic;
}
