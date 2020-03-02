/*
 * 	fl_angel_shader.h
 *
 *	A shader applied to distort the framebuffer after rendering.
 */

#ifndef FL_ANGEL_SHADER_H_
#define FL_ANGEL_SHADER_H_

#include "fl_framebuffer_shader.h"

class FLAngelShader : public virtual FLFramebufferShader {
	public:
		FLAngelShader();

		virtual bool create_program( std::string program_name );

		void set_dx( float dx );
		void set_dy( float dy );
		void set_radius( float radius );
	protected:
		GLint dx_location;		
		GLint dy_location;		
		GLint radius_location;		
};


#endif

