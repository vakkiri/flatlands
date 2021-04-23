/*
 * 	fl_projectiles.h
 *
 * 	Moving things which do damage.
 *
 */

#ifndef FL_PROJECTILES_H_
#define FL_PROJECTILES_H_

#include <string>
#include "common/basic_types.h"

struct FLProjectileInfo {
	float v;
	float x;
	float y;
	float w;
	float h;
	int damage;
	int life;
	float gravity_scale;
	std::string collection;
};

namespace FLProjectiles {
	void define(std::string name, float v, float x, float y, float w, float h, int damage, int life, float gravity_scale, std::string collection);
	void create(float x, float y, std::string type, bool player_is_source, FLDirection direction);
}

#endif
