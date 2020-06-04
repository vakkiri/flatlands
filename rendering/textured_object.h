/*
 * 	textured_object.h
 *
 * 	Base abstract class for any object which will be rendered with a
 * 	texture.
 *
 */

#ifndef TEXTURED_OBJECT_H_
#define TEXTURED_OBJECT_H_

#include "../common/basic_types.h"
#include "../common/game_object.h"
#include <vector>

struct FLTexturedObjectParams {
	FLGameObject *owner;
	float x;
	float y;
	float w;
	float h;
};

class FLTexturedObject {
  public:
	FLTexturedObject() = delete;
	FLTexturedObject(FLGameObject *parent, float w, float h);
	FLTexturedObject(FLGameObject *owner, float x, float y, float w, float h);
	FLTexturedObject(float x, float y, float w, float h);
	FLTexturedObject(FLTexturedObjectParams &params);

	virtual ~FLTexturedObject(){};

	float x();
	float y();
	float w();
	float h();
	virtual float s();
	virtual float t();

	void set_st(float s, float t);
	void set_x(float x);
	void set_y(float y);
	void set_w(float w);
	void set_h(float h);

	void set_reverse(bool reverse);
	bool reversed();

	void set_visible(bool visible);
	bool is_visible();

  protected:
	FLGameObject *owner;
	float _x;
	float _y;
	float _w;
	float _h;
	float _s;
	float _t;
	bool reverse;
	bool visible;
};

#endif
