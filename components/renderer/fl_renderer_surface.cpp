/*
 * 	fl_renderer_surface.cpp
 *
 */

#include "fl_renderer_surface.h"

FLRendererSurface::FLRendererSurface() {
}

void FLRendererSurface::push(FLRenderer* r) {
	render_queue.push(r);
}

void FLRendererSurface::render() {
	/*
	 * This works in two passes:
	 *
	 * First, add all vertices etc. from each object to their respective shaders.
	 *
	 * Second, we call draw_one() on each shader in order to do the actual rendering.
	 *
	 */
	/*
	 *	Actually developing out and using this new component is on hold until
	 *	i decide how to handle the fact that different shaders will sometimes
	 *	require different sets of parameters. FLRenderer::render may need to
	 *	be configurable.
	 */
	std::vector<FLShader*> shader_queue;
	while (!render_queue.empty()) {
		render_queue.top()->render();
		render_queue.pop();
	}

	while (!shader_queue.empty()) {
		//shader_queue->back()->render_one();
		shader_queue.pop_back();
	}

}

