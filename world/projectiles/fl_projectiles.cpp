/*
 *	fl_projectiles.cpp
 *
 */

#include <functional>

#include "fl_projectiles.h"
#include "../player/player.h"
#include "../../components/components.h"
#include "../../environment/fl_environment.h"
#include "../../rendering/renderer.h"

FLAnimatedObjectParams reep_animation_params = {
	1,
	4,
	5,
	16,
	16,
	true
};

FLProjectile::FLProjectile( 	float x, float y, float w, float h, 
				float vx, float vy, 
				int damage, 
				FLAnimatedObjectParams animation_params ) : 
				FLGameObject( x, y, w, h ) {
	FLTexturedObjectParams tex_params = { this, 0, 0, w, h };

	add_collider( "position", "body" );
	fl_add_collider_to_group( colliders["body"], "projectiles" );
	fl_get_collider( colliders["body"] )->set_collision_method( std::bind( &FLProjectile::on_collision, this, std::placeholders::_1 ) );

	physics_handler_handle = new_physics_handler( this, "body" );
	animators["body"] = new FLAnimatedObject( tex_params, animation_params );
	this->damage = damage;	

	Renderer::getInstance().add_to_world( animators["body"] );
	physics_handler()->set_gravity_factor(0.f);
	physics_handler()->set_friction_factor(0.f);
	physics_handler()->accelerate( vx, vy );
}

FLProjectile::~FLProjectile() {
	Renderer::getInstance().remove_from_world( animators["body"] );
	environment()->player()->hit( damage );
}

void FLProjectile::on_collision( FLCollider* obj ) {
	delete this;
}

FLReepProjectile::FLReepProjectile( float x, float y, float vx, float vy ) 
	: FLProjectile( x, y, 16, 16, vx, vy, 10, reep_animation_params ) {
		animators["body"]->set_st(256, 288);
}

