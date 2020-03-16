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

#define FULL_COOLDOWN 300
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

	// Surface for attack effect
	// We maintain one surface instead of creating one "per effect"
	// since the cost of generating particle systems is much higher
	// than modifying an existing one.
	
	attack_surface = new FLLightningParticleSurface();
	Renderer::getInstance().add_particle_surface( attack_surface );
	attack_cooldown = 0;
}

FLRhombus::~FLRhombus() {
	Renderer::getInstance().remove_from_world( this );
	delete attack_surface;
}

void FLRhombus::collide_with( FLPlayer *player ) {
	player->reset();
}

float FLRhombus::x() {
	return FLGameObject::x();
}

float FLRhombus::y() {
	return FLGameObject::y() + ( movement_radius * sin(phase) );
}

void FLRhombus::update() {
	unsigned int tick = SDL_GetTicks();
	phase = speed * (float(tick + offset) / 1000.f);

	attack_cooldown--;

	if ( attack_cooldown < 0 ) {
		// Add particles from orb to orb
		for ( int i = 0; i < 7; ++i ) {
			float vx = ((float(rand() % 1000) / 1000.f) - 0.5f) * 1.f;
			float vy = (float(rand() % 1000) / 1000.f) * -1.f;
			attack_surface->add_particle(x(), y(), vx, vy);
		}
		attack_cooldown = FULL_COOLDOWN;
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


	static unsigned int offset_phase = 777;
	offset_phase += 666;
	offset = offset_phase;
	phase = 0;
	speed = 3.f;

	movement_radius = 32.f;
}

FLSmallrhombus::~FLSmallrhombus() {
	Renderer::getInstance().remove_from_world( this );
}

void FLSmallrhombus::collide_with( FLPlayer *player ) {
	player->reset();
}

void FLSmallrhombus::update() {
	unsigned int tick = SDL_GetTicks();
	phase = speed * (float(tick + offset) / 1000.f);
}

float FLSmallrhombus::x() {
	return parent->x() + 8.f + ( (movement_radius - (24.f * sin(phase/4.f))) * cos(phase) );
}

float FLSmallrhombus::y() {
	return parent->y() + 8.f + ( (movement_radius - (24.f * sin(phase/4.f))) * sin(phase) );
}
