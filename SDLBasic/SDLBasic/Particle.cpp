#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include "Particle.h"
#include <stdlib.h>
#include <math.h>

// inicijalno postavi cesticu na sred ekrana
Particle::Particle()
{
	Init();
}

Particle::~Particle()
{

}

// azuriraj poziciju pomocu XSpeed i YSpeed koji su dobili neke random vrednosti u konstruktoru
void Particle::Update(int Interval)
{
	// u svakom updateu mu daj malo dodatnog zaokreta
	_Direction += 0.0002 * Interval;

	// projekcija brzine na X i Y pravac
	double XSpeed = _Speed * cos(_Direction);
	double YSpeed = _Speed * sin(_Direction);

	// azuriraj trenutne pozicije cestica;
	// mnozi sa Intervalom da bi na svim brzinama graficke bilo isto ponasanje
	Particle_X += XSpeed * Interval;
	Particle_Y += YSpeed * Interval;

	if (Particle_X < -1 || Particle_X > 1 || Particle_Y < -1 || Particle_Y > 1)
		Init();

	if (rand() < RAND_MAX / 500)
		Init();
}

void Particle::Init()
{
	Particle_X = 0;
	Particle_Y = 0;

	// ugao nesto random izmedju 0 i 2PI, brzina nesto izmedju 0 i konstante s kojom se mnozi
	_Direction = (2 * M_PI * rand()) / RAND_MAX;
	_Speed = (0.03 * rand()) / RAND_MAX;

	_Speed *= _Speed;
}