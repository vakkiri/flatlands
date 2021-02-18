/*
 * 	rendered_surface.h
 *
 * 	Base structures holding information for any polygon which will be
 * 	rendered by a shader.
 *
 */

#ifndef RENDERED_SURFACE_H_
#define RENDERED_SURFACE_H_

#include "../common/basic_types.h"

#include "renderable.h"

#include <SDL2/SDL_opengl.h>
#include <string>
#include <vector>

struct texture;
class FLTexturedObject;

class FLTexturedRectShader;
class FLColoredPolyShader;

class FLRenderedSurface : public FLRenderable {
  public:
	FLRenderedSurface();
	virtual void render() = 0;

  protected:
	GLuint vbo;
	GLuint ibo;
	GLuint vao;
	unsigned int num_indices;
	unsigned int num_verts;
};

class FLColoredSurface : public FLRenderedSurface {
  public:
	FLColoredSurface();
	virtual void render();

	virtual void clear_verts();
	virtual void update_buffers();
	virtual void add_verts(std::vector<fl_colored_vertex> &new_verts);

	void set_shader(FLColoredPolyShader *shader);

  protected:
	std::vector<fl_colored_vertex> verts;
	std::vector<unsigned int> indices;
	FLColoredPolyShader *shader;
};

class FLTexturedSurface : public FLRenderedSurface {
  public:
	FLTexturedSurface();
	virtual void render();
	virtual void update_buffers(std::vector<FLTexturedObject *> &objects);
	virtual void update_buffers(std::vector<FLTexturedObject> &objects);
	virtual void update_buffers(std::vector<int> &ids);
	virtual void update_buffers(FLTexturedObject *object);

	void set_tex(texture *tex);
	void set_tex(std::string name);
	void set_shader(FLTexturedRectShader *shader);

  protected:
	texture *tex;
	FLTexturedRectShader *shader;
};

#endif
