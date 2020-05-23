/*
 * 	fl_monster.cpp
 *
 */

#include "fl_monster.h"
#include "../../rendering/renderer.h"
#include "../../components/components.h"

FLMonster::FLMonster( float x, float y, float w, float h, FLAnimatedObjectParams animation_params ) :
	FLGameObject( x, y, w, h ) {
	
	FLTexturedObjectParams tex_params = { this, 0, 0, w, h };


	add_collider( "position", "tilemap" );
	physics_handler_handle = new_physics_handler( this, "tilemap" );
	updator_handle = new_updator( this );

	animators["body"] = new FLAnimatedObject( tex_params, animation_params );
	Renderer::getInstance().add_to_world( animators["body"] );
	monster_state = FL_MONSTER_IDLE;	
	facing_right = true;

}

