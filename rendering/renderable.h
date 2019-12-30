/*
 *	renderable.h
 *
 *	Base class for items which should be rendered via a render() call.
 *
 */

#ifndef RENDERABLE_H_
#define RENDERABLE_H_

class Renderer;
class FLRenderedSurface;

class FLRenderable {
	public:
		FLRenderable(Renderer &r);
		virtual ~FLRenderable();

		virtual void render() = 0;
	protected:
		FLRenderedSurface *surface;
		bool visible;
};

#endif
