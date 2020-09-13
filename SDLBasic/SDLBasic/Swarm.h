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

	// ni ptr da pokazuje ni na sta, ni Particle na koji pokazuje ne moze da se menja
	inline const Particle * const GetParticles() { return pSwarm_Particle; }

	void Update(int TimeElapsed);
};

