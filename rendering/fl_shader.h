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

class FLShader {
	public:
		FLShader();
		FLShader(std::string name);
		virtual ~FLShader();

		bool create_program(std::string program_name);
		bool create_program();

		bool bind();
		void unbind();

		GLuint get_program();
	protected:
		GLuint load_shader(const GLchar* source[], GLenum shader_type);
		GLuint load_shader(std::string path, GLenum shader_type);

		std::string name;
		GLuint program_id;

};

#endif

