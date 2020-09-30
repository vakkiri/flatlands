/*
 *	fl_projectiles.cpp
 *
 */

#include <functional>

#include "../../components/components.h"
#include "../../environment/fl_environment.h"
#include "../../rendering/renderer.h"
#include "../player/player.h"
#include "../monster/fl_monster.h"
#include "fl_projectiles.h"

FLAnimatedObjectParams reep_animation_params = {1, 8, 1, 16, 16, true};
FLAnimatedObjectParams fusion_animation_params = {1, 1, 1, 16, 8, true};

FLProjectile::FLProjectile(float x, float y, float w, float h, float vx,
						   float vy, int damage,
						   FLAnimatedObjectParams animation_params)
	: FLGameObject(x, y, w, h) {

	FLTexturedObjectParams tex_params = {shapes["position"], 0, 0, w, h};
	animators["body"] = new FLAnimatedObject(tex_params, animation_params);
	Renderer::getInstance().add_to_world(animators["body"]);

	FLShape *collision_shape =
		new FLShape(x + (0.25 * w), y + (0.25 * h), w * 0.5, h * 0.5);
	shapes.insert(std::make_pair("collision", collision_shape));

	updator_handle = new_updator(this);

	add_collider("collision", "body");
	fl_add_collider_to_group(colliders["body"], "projectiles");
	fl_get_collider(colliders["body"])
		->set_collision_method(std::bind(&FLProjectile::on_collision, this,
										 std::placeholders::_1));

	physics_handler_handle = new_physics_handler(this, "body");
	this->damage = damage;
	physics_handler()->set_gravity_factor(0.f);
	physics_handler()->set_friction_factor(0.f);
	physics_handler()->accelerate(vx, vy);
	hits_player = true;
	die_on_stop = true;
}

FLProjectile::~FLProjectile() {
	Renderer::getInstance().remove_from_world(animators["body"]);
}

void FLProjectile::per_frame_update() {
	--life;

	if (life <= 0) {
		delete this;
	} else if (die_on_stop and stationary()) {
		delete this;
	}
}

void FLProjectile::on_collision(FLCollider *obj) {
	// TODO: this is clearly an uncomplete implementation lol
	// need to check the obj group and hit either player, enemy or neither
	(void)obj;
	if (obj != nullptr) {
		if (hits_player && obj->is_in_group("player")) {
			environment()->player()->hit(damage);
			delete this;
		} else {
			if (obj->is_in_group("monsters")) {
				((FLMonster*)(obj->get_owner()))->hit(damage);
				delete this;
			}
		}
	}
}

FLReepProjectile::FLReepProjectile(float x, float y, float vx, float vy)
	: FLProjectile(x, y, 16, 16, vx, vy, 50, reep_animation_params) {
	animators["body"]->set_st(256, 288);
	life = 360;
}

FLFusionProjectile::FLFusionProjectile(float x, float y, float vx, float vy)
	: FLProjectile(x, y, 16, 8, vx, vy, 50, fusion_animation_params) {
	animators["body"]->set_st(544, 144);
	life = 25;
	hits_player = false;
	physics_handler()->unbound_velocity();
	fl_get_collider(colliders["body"])->add_target_collision_group("monsters");
}

bool FLProjectile::stationary() {
	return physics_handler()->xvel() == 0 && physics_handler()->yvel() == 0;
}

