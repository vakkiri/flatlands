/*
 * 	fl_shader.h
 *
 * 	Flatlands shader class. A minimal shader will
 * 	contain a programID; can be extended to include
 * 	specific shader variable access.
 *
 */

#ifndef FL_SHADER_H_
#define FL_SHADER_H_

#include <string>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>

class FLShader {
	public:
		FLShader();
		virtual ~FLShader();

		virtual bool create_program(std::string program_name);

		void set_camera( glm::mat4 matrix );
		void set_projection( glm::mat4 matrix );
		void update_camera();
		void update_projection();
		bool bind();
		void unbind();

		GLuint get_program();
	protected:
		GLuint load_shader(const GLchar* source[], GLenum shader_type);
		GLuint load_shader(std::string path, GLenum shader_type);

		GLuint program_id;

		glm::mat4 projection_matrix;
		glm::mat4 camera_matrix;
		GLint projection_matrix_location;
		GLint camera_matrix_location;
};

#endif

