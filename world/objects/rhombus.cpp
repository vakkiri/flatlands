/*
 * 	rhombus.cpp
 *
 */

#include <SDL2/SDL.h>
#include <math.h>
#include <iostream>
#include <time.h>

#include "rhombus.h"

#include "../../rendering/renderer.h"
#include "../../rendering/fl_lightning_particle_surface.h"

#include "../world_environment.h"
#include "../player/player.h"

#define NUM_ANIMATIONS 1
#define NUM_STEPS 8
#define FRAMES_PER_STEP 2
#define STEP 16
#define REPEATS true

#define S 64
#define T 64

#define SMALL_S 208
#define SMALL_T 64
#define SMALL_SIZE 8

#define SIZE 16

#define FULL_COOLDOWN 1
#define NUM_BALLS 3

FLRhombus::FLRhombus( float x, float y ) :
	FLGameObject( x, y, SIZE, SIZE ),
	FLAnimatedObject(
			NUM_ANIMATIONS,
			NUM_STEPS,
			FRAMES_PER_STEP,
			STEP,
			REPEATS
		),
	FLDynamicObject() {
	set_st( S, T );
 
	Renderer::getInstance().add_to_world( this );

	movement_radius = 32.f;
	speed = 2.f;

	static unsigned int offset_phase = 777;
	offset_phase += 777;
	offset = offset_phase;
	phase = 0;

	// Add the balls

	for ( int i = 0; i < NUM_BALLS; ++i )	
		children.push_back( new FLSmallrhombus(this) );

	cur_ball = 0;

	// Surface for attack effect
	// We maintain one surface instead of creating one "per effect"
	// since the cost of generating particle systems is much higher
	// than modifying an existing one.
	
	attack_surface = new FLLightningParticleSurface();
	Renderer::getInstance().add_particle_surface( attack_surface );

	attack_cooldown = 0;
	attack_pos = 0.f;
	attack_step = 0.01f;
}

FLRhombus::~FLRhombus() {
	Renderer::getInstance().remove_from_world( this );
	delete attack_surface;
}

void FLRhombus::collide_with( FLPlayer *player ) {
	player->reset();
}

float FLRhombus::x() {
	return FLGameObject::x() + ( movement_radius * sin(phase) );
}

float FLRhombus::y() {
	return FLGameObject::y();
}

void FLRhombus::update() {
	unsigned int tick = SDL_GetTicks();
	phase = speed * (float(tick + offset) / 1000.f);

	if ( attack_cooldown > 0 )
		attack_cooldown--;
	else if ( attack_pos < 1.f ) {
		// Add particles from orb to direction opposite from another orb
		int ball2 = cur_ball + 1;
		if ( ball2 >= NUM_BALLS )
			ball2 = 0;

		float x1 = children[cur_ball]->x() + 4;
		float y1 = children[cur_ball]->y() + 4;
		float x2 = children[ball2]->x() + 4;
		float y2 = children[cur_ball]->y() + 128;
		float dx = x2 - x1;
		float dy = y2 - y1;

		float attack_x = x1 + dx * attack_pos;
		float attack_y = y1 + dy * attack_pos;

		float vx = ((float(rand() % 1000) / 1000.f) - 0.5f) * 1.f;
		float vy = (float(rand() % 1000) / 1000.f) * -1.f;

		attack_surface->add_particle(attack_x, attack_y, vx, vy);

		attack_pos += attack_step;

	}
	else {
		attack_pos = 0.f;
		attack_cooldown = FULL_COOLDOWN;
		if ( ++cur_ball >= NUM_BALLS )
			cur_ball = 0;
	}
	
}

FLSmallrhombus::FLSmallrhombus( FLRhombus* parent ) :
	FLGameObject( 0, 0, SMALL_SIZE, SMALL_SIZE ),
	FLAnimatedObject(
			NUM_ANIMATIONS,
			4,
			4,
			SMALL_SIZE,
			REPEATS
		),
	FLDynamicObject() {
	set_st( SMALL_S, SMALL_T );

	Renderer::getInstance().add_to_world( this );
	this->parent = parent;


	static float offset_phase = 1;
	offset_phase += 1.6;
	offset = offset_phase;
	phase = 0;
	speed = 4.f;

	movement_radius = 16.f;
	last_tick = SDL_GetTicks();
}

FLSmallrhombus::~FLSmallrhombus() {
	Renderer::getInstance().remove_from_world( this );
}

void FLSmallrhombus::collide_with( FLPlayer *player ) {
	player->reset();
}

void FLSmallrhombus::update() {
	unsigned int tick = SDL_GetTicks();
	phase += (speed * float(tick - last_tick) / 1000.f);
	last_tick = tick;
}

float FLSmallrhombus::x() {
	return parent->x() + 4.f + ( movement_radius * cos(phase + offset) );
}

float FLSmallrhombus::y() {
	return parent->y() + 4.f + ( movement_radius * sin(phase + offset) );
}
