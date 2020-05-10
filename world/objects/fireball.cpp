/*
 * 	fireball.cpp
 *
 */

#include <SDL2/SDL.h>
#include <math.h>
#include <iostream>
#include <time.h>

#include "fireball.h"

#include "../../rendering/renderer.h"

#include "../world_environment.h"
#include "../player/player.h"

#define NUM_ANIMATIONS 1
#define NUM_STEPS 8
#define FRAMES_PER_STEP 2
#define STEP 16
#define REPEATS true

#define S 64
#define T 48

#define SMALL_S 208
#define SMALL_T 48
#define SMALL_SIZE 8

#define SIZE 16

FLFireball::FLFireball( float x, float y ) :
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
	last_tick = 0;
	cooldown = 400;
}

FLFireball::~FLFireball() {
	Renderer::getInstance().remove_from_world( this );
}

void FLFireball::collide_with() {
	FLWorldEnvironment::getInstance().player()->reset();
}

float FLFireball::x() {
	return FLGameObject::x() + ( movement_radius * cos(phase) );
}

float FLFireball::y() {
	return FLGameObject::y() + ( movement_radius * sin(phase) );
}

void FLFireball::update() {
	unsigned int tick = SDL_GetTicks();

	phase = speed * (float(tick + offset) / 1000.f);

	if ( tick - last_tick > cooldown ) {
		FLSmallball* new_ball = new FLSmallball( x() + 8, y() + 8 );
		new_ball->set_velocity( cos(phase), sin(phase) );
		last_tick = tick;
	}
}

FLSmallball::FLSmallball( float x, float y ) :
	FLGameObject( x, y, SMALL_SIZE, SMALL_SIZE ),
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
	life = 300;
}

FLSmallball::~FLSmallball() {
	Renderer::getInstance().remove_from_world( this );
}

void FLSmallball::collide_with() {
	FLWorldEnvironment::getInstance().player()->reset();
}

void FLSmallball::update() {
	position.x += velocity.x;
	position.y += velocity.y;

	if ( --life <= 0 )
		zombie = true;
}

void FLSmallball::set_velocity( float x, float y ) {
	velocity.x = x;
	velocity.y = y;
}

