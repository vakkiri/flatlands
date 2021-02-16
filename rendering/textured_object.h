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
#include "../common/fl_shape.h"
#include <vector>

class FLTexturedObject;

int new_texturer();
int new_texturer(float x, float y, float w, float h, float s, float t);
FLTexturedObject* get_texturer(int handle);
void delete_texturer(int handle);

struct FLTexturedObjectParams {
	FLShape *parent;
	float x;
	float y;
	float w;
	float h;
};

class FLTexturedObject {
  public:
	FLTexturedObject();
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

	void set_alive();
	void kill();
	bool alive();

  protected:
	FLShape *parent;
	float _x;
	float _y;
	float _w;
	float _h;
	float _s;
	float _t;
	bool reverse;
	bool visible;
	bool _alive;
};

#endif
