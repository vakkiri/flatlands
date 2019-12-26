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
		FLShader(std::string name);
		virtual ~FLShader();

		virtual bool create_program(std::string program_name);
		virtual bool create_program();

		void set_projection( glm::mat4 matrix );
		void update_projection();
		void set_modelview( glm::mat4 matrix );
		void update_modelview();
		bool bind();
		void unbind();

		GLuint get_program();
	protected:
		GLuint load_shader(const GLchar* source[], GLenum shader_type);
		GLuint load_shader(std::string path, GLenum shader_type);

		std::string name;
		GLuint program_id;

		glm::mat4 projection_matrix;
		GLint projection_matrix_location;

		glm::mat4 modelview_matrix;
		GLint modelview_matrix_location;
};

#endif

