#pragma once

#include <iostream>
#include <SDL.h>
#include "Screen.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "Swarm.h"
using namespace std;

/*
	when building, some main-related error occured, and the word "main"
	was purple, like it was defined somewhere; used GoToDefinition, which
	led to SDL_main.h, where it stated:
		#if defined(SDL_MAIN_NEEDED) || defined(SDL_MAIN_AVAILABLE)
		#define main    SDL_main
		#endif
	it says that main should be defined if main was needed OR available,
	which didn't make much sense, so the second condition was added negation;
	if it ISN'T AVAILABLE, define main:
		#if defined(SDL_MAIN_NEEDED) || !defined(SDL_MAIN_AVAILABLE)
		#define main    SDL_main
		#endif
	which solved the building problem
*/

/*
	bitwise &
 	0xXYZABC
  & 0xFF0000
  = 0xXY0000
  FF0000 is a mask. FF in binary are all ones in 8 places (for 1 byte);
  that way, it is overlapped with the thing masked and it returns values only in those places that overlap;
  because everything else is 0, all the other places will be 0; in the example above, it returns only XY, and the rest
  doesn't satisfy logical AND operation;

  more simple way: bithifting
  c = 0xXYZABC;
  c >> 16;
  c is now 0xXY (0x0000XY), because everything is shifted for 2 bytes;
  in hexadec, 2 places are needed for 1 byte; this way, all is shifted 4 places and it comes to the end,
  and what was at the end is thrown away
*/

int main()
{
	// for rand() to avoid the same pseudo random sequence with every running of the program
	// srand is seeded with some number - with time, in this case
	srand(time(NULL));

	Screen Screen;
	const int ScreenHalfWidth = Screen.WINDOW_WIDTH / 2;
	const int ScreenHalfHeight = Screen.WINDOW_HEIGHT / 2;

	// if initialization wasn't correct, print a message
	if (!Screen.Init())
		cout << "Error initializing SDL!" << endl;

	Swarm Swarm;

	// main loop for program execution
	while (true)
	{
		int TimeElapsed = SDL_GetTicks();

		Swarm.Update(TimeElapsed);

		// shades for R, G, B follow sinusoidal change
		unsigned char Red = (sin(TimeElapsed * 0.0005) + 1) / 2 * 255;
		unsigned char Green = (sin(TimeElapsed * 0.001) + 1) / 2 * 255;
		unsigned char Blue = (sin(TimeElapsed * 0.0015) + 1) / 2 * 255;

		// get all of NPARTICLES particles, each one gets a location on the screen,
		// whose intiialization is defined in the constructor Particle();
		// dodajemo 1 da bi opseg presao sa -1:1 na 0:2, pa mnozimo sa polovinom visine/sirine;
		const Particle * const pParticle = Swarm.GetParticles();
		for (size_t i(0); i < Swarm::NPARTICLES; ++i)
		{
			Particle Particle = pParticle[i];
			// x and y are multiplied with window width, so that movement would be
			// circular, and not oval
			int x = (Particle.Particle_X + 1) * ScreenHalfWidth;
			int y = Particle.Particle_Y * ScreenHalfWidth + ScreenHalfHeight;

			Screen.SetPixel(x, y, Red, Green, Blue);
		}

		Screen.BoxBlur();

		// refresh screen
		Screen.Update();
		
		// if ProcessEvent returns false (the user presses X), break the main loop
		if (!Screen.ProcessEvent())
			break;
		
	}

	// clear all pointers
	Screen.Clear();
	


	return 0;
}