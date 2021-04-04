/*
 * 	fl_camera.cpp
 *
 */

#include <iostream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "fl_camera.h"
#include "renderer.h"
#include "../common/game_object.h"

FLCamera::FLCamera() {
	this->min_x = 0;
	this->min_y = 0;
	this->max_x = 2048;
	this->max_y = 2048;
	parent = nullptr;
	parallax_x = 1.0;
	parallax_y = 1.0;
	camera = glm::mat4(1.0);
}

void FLCamera::set_scale(float scale) {
	camera[0][0] = scale;
	camera[1][1] = scale;
}

void FLCamera::set_parent(FLGameObject *obj) {
	parent = obj;
}

void FLCamera::set_min_x(float x) { min_x = x; }

void FLCamera::set_min_y(float y) { min_y = y; }

void FLCamera::set_max_x(float x) { max_x = x; }

void FLCamera::set_max_y(float y) { max_y = y; }

float FLCamera::x() { return camera[3][0]; }

float FLCamera::y() { return camera[3][1]; }

float FLCamera::x_scale() { return camera[0][0]; }

float FLCamera::y_scale() { return camera[1][1]; }

glm::mat4 FLCamera::mat() { return camera; }

void FLCamera::reset() {
	if (parent != nullptr) {
		set_pos(parent->x(), parent->y());
	} else {
		set_pos(min_x, min_y);
	}
}

void FLCamera::set_pos(float new_x, float new_y) {
	// TODO: subtract screen_width / 2, screen_height / 2
	float dx = x() + new_x;
	float dy = y() + new_y;

	camera = glm::translate(camera, glm::vec3(-dx, -dy, 0));
}

void FLCamera::bound() {
	// this looks weird because all the values are... negative...
	camera[3][0] = std::min(camera[3][0], -min_x);
	camera[3][0] = std::max(camera[3][0], -max_x);
	camera[3][1] = std::min(camera[3][1], -min_y);
	camera[3][1] = std::max(camera[3][1], -max_y);
	std::cout << -max_x << std::endl;
	std::cout << -max_y << std::endl;
	std::cout << "-" << std::endl;
}

void FLCamera::update() {
	if (parent != nullptr) {
		float xamt = 0;
		float yamt = 0;

		float dx = x() / x_scale() + parent->x() - (Renderer::getInstance().get_screen_width() / 4.f);
		float dy = y() / y_scale() + parent->y() - (Renderer::getInstance().get_screen_height() / 4.f);

		dy -= 48;

		xamt = (dx / 50) * -5.0;	// uhhh i should make these variables
		yamt = (dy / 50) * -5.0;

		camera = glm::translate(camera, glm::vec3(xamt, yamt, 0));
	}

	bound();
}


