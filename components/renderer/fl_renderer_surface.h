/*
 * 	fl_renderer_surface.h
 *
 * 	surface which holds rendering information before being sent to shader
 *
 */

#ifndef FL_RENDERER_SURFACE_H_
#define FL_RENDERER_SURFACE_H_

#include <queue>
#include <vector>
#include <functional>

#include "fl_renderer.h"

class FLRendererSurface {
	public:
		FLRendererSurface();

		void push(FLRenderer* r);
		void render();
	protected:
		std::priority_queue<FLRenderer*, std::vector<FLRenderer*>, compare_renderers> render_queue;
};

#endif

