/*
 * 	fl_shader.cpp
 *
 * 	Basic shader loading and functionality.
 *
 */

#include <glm/ext.hpp>
#include <iostream>
#include <fstream>

#include "../logging/logging.h"
#include "fl_shader.h"

#define NO_PROGRAM 0

FLShader::FLShader() {
	program_id = NO_PROGRAM;
}

FLShader::~FLShader() {
	if ( program_id )
		glDeleteProgram( program_id );
}

bool FLShader::bind() {
	GLenum error;

	glUseProgram( program_id );

	error = glGetError();
	if ( error != GL_NO_ERROR ) {
		log_error( "Error binding shader." );
		std::cout << error;
		return false;
	}

	return true;
}

void FLShader::unbind() {
	glUseProgram( NO_PROGRAM );
}

GLuint FLShader::load_shader(const GLchar* source[], GLenum shader_type) {
	GLuint shader = glCreateShader( shader_type );
	glShaderSource( shader, 1, source, NULL );
	glCompileShader( shader );

	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &vShaderCompiled );

	if ( vShaderCompiled != GL_TRUE ) {
		log_error("Unable to compile shader.");
        int log_len = 0;
        int max_len = 0;
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &max_len );
        char *log = new char[ max_len ];
        glGetShaderInfoLog( shader, max_len, &log_len, log );
        if ( log_len > 0 )
            printf( "%s\n", log );
        delete [] log;
		return 0;
	}

	glAttachShader( program_id, shader );


	return shader;
}

GLuint FLShader::load_shader(std::string path, GLenum shader_type) {
	GLuint shader = 0;
	std::string shader_string;
	std::ifstream infile( path.c_str() );

	if ( infile ) {
		shader_string.assign( std::istreambuf_iterator<char>( infile ), std::istreambuf_iterator<char>() );
        //shader_string += '\0';
		const GLchar* source = shader_string.c_str();
		shader = load_shader((const GLchar**)&source, shader_type);
	}
	else
		log_error("Could not open shader from file: " + path);

	return shader;
}

bool FLShader::create_program(std::string program_name) {
	std::string v_path = "shaders/" + program_name + ".glvs";
	std::string f_path = "shaders/" + program_name + ".glfs";

	program_id = glCreateProgram();

	GLuint vertex_shader = load_shader( v_path, GL_VERTEX_SHADER );

	if ( vertex_shader == 0 ) {
		log_error("Error loading vertex shader.");
		glDeleteProgram( program_id );
		program_id = NO_PROGRAM;
		return false;
	}

	GLuint fragment_shader = load_shader( f_path, GL_FRAGMENT_SHADER );

	if ( fragment_shader == 0 ) {
		log_error("Error loading fragment shader.");
		glDeleteShader( vertex_shader );
		glDeleteProgram( program_id );
		program_id = NO_PROGRAM;
		return false;
	}

	glLinkProgram( program_id );

	GLint program_success = GL_TRUE;
	glGetProgramiv( program_id, GL_LINK_STATUS, &program_success );
	if ( program_success != GL_TRUE ) {
		log_error("Error linking shader program");
		glDeleteShader( vertex_shader );
		glDeleteShader( fragment_shader );
		glDeleteProgram( program_id );
		program_id = NO_PROGRAM;
		return false;
	}

	// Delete lingering references
	glDeleteShader( vertex_shader );
	glDeleteShader( fragment_shader );

	pc_matrix_location = glGetUniformLocation( program_id, "pcMatrix");

	if ( pc_matrix_location == -1 ) {
		log_error( "Could not load shader pc matrix uniform" );
		return false;
	}

	return true;
}

GLuint FLShader::get_program() {
	return program_id;
}

void FLShader::set_camera( glm::mat4 matrix ) {
	camera_matrix = matrix;
}

void FLShader::set_projection( glm::mat4 matrix ) {
	projection_matrix = matrix;
}

void FLShader::update_pc_matrix() {
	glm::mat4 pc_matrix = projection_matrix * camera_matrix;
	glUniformMatrix4fv( pc_matrix_location, 1, GL_FALSE, glm::value_ptr( pc_matrix ));
}

