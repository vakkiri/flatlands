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

	// It has three balls
	new FLSmallrhombus( this );
	new FLSmallrhombus( this );
	new FLSmallrhombus( this );
}

FLRhombus::~FLRhombus() {
	Renderer::getInstance().remove_from_world( this );
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
