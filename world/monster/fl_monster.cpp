/*
 * 	fl_monster.cpp
 *
 */

#include "fl_monster.h"
#include "../player/player.h"
#include "../../components/components.h"
#include "../../environment/fl_environment.h"
#include "../../game/fl_game.h"
#include "../../rendering/renderer.h"

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

point FLMonster::distance_from_player() {
	// TODO: choose closest of all net players
	FLPlayer* player = FLGame::instance().environment()->player();
	point p;
	p.x = x() - player->x();
	p.y = y() - player->y();
	return p;
}

