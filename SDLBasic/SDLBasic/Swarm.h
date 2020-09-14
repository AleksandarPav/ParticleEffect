#pragma once

#include "Particle.h"

class Swarm {

public:
	const static unsigned NPARTICLES = 5000;

private:
	Particle *pSwarm_Particle;
	int LastTime;

public:
	Swarm();
	virtual ~Swarm();

	// ptr can't be changed, nor the Particle it looks at
	inline const Particle * const GetParticles() { return pSwarm_Particle; }

	void Update(int TimeElapsed);
};

