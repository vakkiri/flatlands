/*
 * 	fl_monster.cpp
 *
 */

#include "fl_monster.h"
#include "../../rendering/renderer.h"

FLMonster::FLMonster( float x, float y, float w, float h, FLAnimatedObjectParams animation_params ) :
	FLAnimatedObject( animation_params ) {

	Renderer::getInstance().add_to_world( this );
	monster_state = FL_MONSTER_IDLE;	
	facing_right = true;

}

