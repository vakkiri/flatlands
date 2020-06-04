/*
 * 	fl_particle_shader.h
 *
 * 	Wrapper for shaders which worth with polygons consisting of
 * 	coloured vertices.
 *
 */

#ifndef FL_PARTICLE_SHADER_H_
#define FL_PARTICLE_SHADER_H_

#include "fl_polygon_shader.h"
#include <string>

class FLParticleShader : public virtual FLPolygonShader {
  public:
	FLParticleShader();

	virtual bool create_program(std::string program_name);
	virtual void render(GLuint vao, unsigned int num_indices);

	void set_life_pointer(GLsizei stride, const GLvoid *data);
	void enable_life_pointer();

  protected:
	GLint life_location;
};

#endif
