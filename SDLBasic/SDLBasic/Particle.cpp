#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include "Particle.h"
#include <stdlib.h>
#include <math.h>

// initially, particle is in the middle of the screen
Particle::Particle()
{
	Init();
}

Particle::~Particle()
{

}

// update position with XSpeed and YSpeed, that got some random values in the constructor
void Particle::Update(int Interval)
{
	// with every update, give it a little extra spin
	_Direction += 0.0002 * Interval;

	// projection of velocity to X and Y direction
	double XSpeed = _Speed * cos(_Direction);
	double YSpeed = _Speed * sin(_Direction);

	// update current particle positions;
	// multiplied with Interval, so that speed of simulation would be constant on every platform
	Particle_X += XSpeed * Interval;
	Particle_Y += YSpeed * Interval;

	// conditions for initializing - going off of screen and some random moment
	if (Particle_X < -1 || Particle_X > 1 || Particle_Y < -1 || Particle_Y > 1)
		Init();

	if (rand() < RAND_MAX / 500)
		Init();
}

void Particle::Init()
{
	// initial position set to zero
	Particle_X = 0;
	Particle_Y = 0;

	// angle is random between 0 i 2*PI, velocity sth between 0 and chosen constants
	_Direction = (2 * M_PI * rand()) / RAND_MAX;
	_Speed = (0.03 * rand()) / RAND_MAX;

	_Speed *= _Speed;
}