/*
 * 	particle.h
 *
 *	Basic particle datatype.
 *
 */

#ifndef FL_PARTICLE_H_
#define FL_PARTICLE_H_

struct fl_particle {
	float x;
	float y;
	float vx;
	float vy;
	unsigned int life;
};

#endif

