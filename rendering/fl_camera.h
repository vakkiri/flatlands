/*
 * 	fl_camera.h
 *
 * 	A camera class.
 *
 */

#ifndef FL_CAMERA_H_
#define FL_CAMERA_H_

#include <glm/glm.hpp>

class FLGameObject;

class FLCamera {
	public:
		FLCamera();
		void set_parent(FLGameObject *obj);
		void set_scale(float scale);
		void set_pos(float x, float y);
		void set_max_x(float x);
		void set_max_y(float y);
		void set_min_x(float x);
		void set_min_y(float y);
		void reset();
		void update();

		float x();
		float y();
		float x_scale();
		float y_scale();

		glm::mat4 mat();
	protected:
		void bound();

		FLGameObject *parent;
		glm::mat4 camera;
		float min_x;
		float min_y;
		float max_x;
		float max_y;
		float parallax_x;
		float parallax_y;

};

#endif

