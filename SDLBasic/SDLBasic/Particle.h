#pragma once

// struct is chosen over class, because particles will have to be accessed in every frame, so they need to be public
struct Particle {

	// coordinates are of type double, so that a smooth movement with decimal values can bi programmed;
	// when showing on screen, coordinates are rounded
	double Particle_X;
	double Particle_Y;

private:

	// particle velocity
	double _Speed;
	// angle in degrees (0 - 360)
	double _Direction;

private:

	void Init();

public:

	Particle();
	virtual ~Particle();

	void Update(int Interval);
};

