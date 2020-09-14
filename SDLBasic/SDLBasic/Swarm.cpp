#include "Swarm.h"
#include <math.h>

Swarm::Swarm() : LastTime(0)
{
	pSwarm_Particle = new Particle[NPARTICLES];
}

Swarm::~Swarm()
{
	delete[] pSwarm_Particle;
}

// calls update for each particle
void Swarm::Update(int TimeElapsed)
{
	int Interval = abs(TimeElapsed - LastTime);

	for (size_t i(0); i < NPARTICLES; ++i)
		pSwarm_Particle[i].Update(Interval);

	LastTime = TimeElapsed;
}