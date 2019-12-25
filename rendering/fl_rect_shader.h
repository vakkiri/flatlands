/*
 * 	fl_rect_shader.h
 *
 * 	A basic shader for rectangles.
 *
 */

#include "fl_shader.h"

class FLRectShader : public FLShader {
	public:
		void update_projection();
		void update_modelview();
	protected:
		GLint vertex_pos_2d_location;
		GLint color_location;

		glm::mat4 projection_matrix;
		GLint projection_matrix_location;

		glm::mat4 model_view_matrix;
		GLint model_view_matrix_location;
};

