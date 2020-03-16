/*
 * 	particle.h
 *
 *	Basic particle datatype.
 *
 */

#ifndef FL_PARTICLE_H_
#define FL_PARTICLE_H_

#include <vector>

struct fl_particle {
	std::vector<float> xs;
	std::vector<float> ys;
	float vx;
	float vy;
	int life;
};

#endif

