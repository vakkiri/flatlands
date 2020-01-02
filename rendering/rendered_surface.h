/*
 * 	rended_surface.h
 *
 * 	Base structures holding information for any polygon which will be
 * 	rendered by a shader.
 *
 */

#ifndef RENDERED_SURFACE_H_
#define RENDERED_SURFACE_H_

#include "../rendering/renderable.h"

#include <SDL2/SDL_opengl.h>
#include <string>
#include <vector>

struct texture;
class Renderer;
class FLTexturedObject;
class FLTexturedRectShader;

class FLRenderedSurface : public FLRenderable {
	public:
		FLRenderedSurface(Renderer& r);
		virtual void render() = 0;
		virtual void update_buffers( std::vector<FLTexturedObject*>& objects ) = 0;

	protected:
		GLuint vbo;
		GLuint ibo;
		GLuint vao;
		unsigned int num_indices;
		unsigned int num_verts;
};

class FLTexturedSurface : public FLRenderedSurface {
	public:
		FLTexturedSurface( Renderer &r );
		virtual void render();
		virtual void update_buffers( std::vector<FLTexturedObject*>& objects );
		virtual void update_buffers( FLTexturedObject* object );

		void set_tex( texture *tex );
		void set_tex( std::string name );
	protected:
		texture *tex;
		FLTexturedRectShader& shader;
};

#endif

