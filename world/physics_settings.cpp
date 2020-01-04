/*
 * 	physics_settings.cpp
 *
 */

#include <iostream>

#include "../logging/logging.h"
#include "physics_settings.h"

FLPhysics::FLPhysics() : 
	_gravity(0.01), 
	_friction(0.01), 
	attr_dict{
		{"gravity", &_gravity},
		{"friction", &_friction}
	}{}

void FLPhysics::set_attribute( std::string name, float val ) {
	if ( attr_dict.find( name ) != attr_dict.end() )
		(*attr_dict[name]) = val;
	else
		log_warning( "Could not find attribute " + name + " in physics dictionary" );
}

