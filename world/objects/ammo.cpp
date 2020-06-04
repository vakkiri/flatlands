/*
 * 	ammo.cpp
 *
 */

#include <functional>
#include <iostream>

#include "../../components/components.h"
#include "../../environment/fl_environment.h"
#include "../../net/fl_net.h"
#include "../../rendering/animated_object.h"
#include "../../rendering/renderer.h"
#include "../player/player.h"
#include "ammo.h"

#define SIZE 8

#define NUM_ANIMATIONS 1
#define NUM_STEPS 4
#define FRAMES_PER_STEP 5
#define STEP 8
#define REPEATS true

FLAmmo::FLAmmo(float x, float y, int weapon_index)
	: FLGameObject(x, y, SIZE, SIZE), FLNetObject() {

	FLAnimatedObjectParams anim_params = {
		NUM_ANIMATIONS, NUM_STEPS, FRAMES_PER_STEP, STEP, STEP, REPEATS};
	FLTexturedObjectParams tex_params = {this, 0, 0, SIZE, SIZE};

	FLAnimatedObject *animation = new FLAnimatedObject(tex_params, anim_params);
	animators["body"] = animation;

	animation->set_st(0, 240 + (8 * (weapon_index - 1)));

	// TODO: should maybe have some pre-defined groups to maintain consistency?
	add_collider("position", "position");
	fl_add_collider_to_group(colliders["position"], "items");
	fl_get_collider(colliders["position"])
		->set_collision_method(
			std::bind(&FLAmmo::on_collision, this, std::placeholders::_1));

	Renderer::getInstance().add_to_world(animation);
	this->weapon_index = weapon_index;

	// weapon index 0 has no ammo since it is FL_NO_WEAPON

	if (weapon_index == 0) {
		num_clips = 0;
	} else if (weapon_index == FL_FUSION) {
		num_clips = 2;
	}
}

FLAmmo::~FLAmmo() {
	Renderer::getInstance().remove_from_world(animators["body"]);
	environment()->player()->add_ammo(weapon_index, num_clips);
}

void FLAmmo::on_collision(FLCollider *obj) {
	(void)obj;
	delete this;
}
