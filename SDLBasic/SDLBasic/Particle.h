#pragma once

// odabrana struktura a ne klasa jer ce stalno morati (na svaki refresh ekrana), za ogroman broj particles,
// da se getuju njihove vrednosti; lakse onda da bude public, na ustrb enkapsulacije
struct Particle {

	// razlog sto su koordinate double jeste taj da mozemo da programiramo neki
	// smooth movement koji ce ukljucivati i decimalne vrednosti, pa cemo da ih roundujemo
	// kad ddje do prikaza na ekran
	double Particle_X;
	double Particle_Y;

private:

	// brzina cestice
	double _Speed;
	// ugao (0 - 360)
	double _Direction;

private:

	void Init();

public:

	Particle();
	virtual ~Particle();

	void Update(int Interval);
};

