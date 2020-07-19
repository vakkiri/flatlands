/*
 * 	xp_orb.cpp
 *
 */

#include <iostream>
#include "xp_orb.h"
#include "../../components/components.h"
#include "../../rendering/renderer.h"
#include "../../rendering/animated_object.h"

#define S 304
#define T 64
#define W 8
#define H 8

static FLAnimatedObjectParams animation_params = {1, 7, 4, W, H, true};

FLXPOrb::FLXPOrb(float x, float y) : FLGameObject(x, y, W, H) {
	std::cout << "x: " << this->x() << " y: " << this->y() << std::endl;
	FLTexturedObjectParams tex_params = {shapes["position"], 0, 0, W, H};
	animators["body"] = new FLAnimatedObject(tex_params, animation_params);
	animators["body"]->set_st(S, T);
	Renderer::getInstance().add_to_world(animators["body"]);

	updator_handle = new_updator(this);
	add_collider("position", "tilemap");
	fl_add_collider_to_group(colliders["tilemap"], "projectiles");
	fl_get_collider(colliders["tilemap"])
		->set_collision_method(std::bind(&FLXPOrb::on_collision, this,
									 	std::placeholders::_1));

	ticks_since_created = 0;
	physics_handler_handle = new_physics_handler(this, "tilemap");
	physics_handler()->set_gravity_factor(0.f);
	physics_handler()->set_friction_factor(0.f);
	//physics_handler()->accelerate();
}

FLXPOrb::~FLXPOrb() {
	Renderer::getInstance().remove_from_world(animators["body"]);
}

void FLXPOrb::per_frame_update() {
	ticks_since_created += 1;
	if (ticks_since_created >= 200) {
		delete this;
	}
}

void FLXPOrb::on_collision(FLCollider *obj) {
	(void) obj;
	std::cout << "HI!!!\n";
}

