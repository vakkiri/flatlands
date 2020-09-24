/*
 * 	chip.cpp
 *
 */

#include <iostream>
#include "chip.h"
#include "../../components/components.h"
#include "../../environment/fl_environment.h"
#include "../../net/fl_net.h"
#include "../../rendering/renderer.h"
#include "../player/player.h"

#define S	368
#define T	64
#define SIZE	16

FLChip::FLChip(float x, float y) : FLGameObject(x, y, SIZE, SIZE), FLNetObject() {
	FLAnimatedObjectParams animation_params = {1, 11, 4, SIZE, SIZE, true};
	FLTexturedObjectParams tex_params = {shapes["position"], 0, 0, SIZE, SIZE};
	animators["body"] = new FLAnimatedObject(tex_params, animation_params);
	animators["body"]->set_st(S, T);

	add_collider("position", "position");
	fl_add_collider_to_group(colliders["position"], "items");
	fl_get_collider(colliders["position"])
		->set_collision_method(
				std::bind(&FLChip::on_collision, this, std::placeholders::_1));
	Renderer::getInstance().add_to_world(animators["body"]);

}

FLChip::~FLChip() {
	Renderer::getInstance().remove_from_world(animators["body"]);
}

void FLChip::on_collision(FLCollider *obj) {
	(void)obj;
	environment()->player()->add_chip();
	delete this;
}
