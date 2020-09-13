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
	pri bildovanju stalno javljao neku gresku vezanu za main, a takodje
	i rec "main" bila ljubicasta, kao da je vec definisana negde;
	isao u GoToDefinition, sto me odvelo u SDL_main.h, gde je stajalo:
		#if defined(SDL_MAIN_NEEDED) || defined(SDL_MAIN_AVAILABLE)
		#define main    SDL_main
		#endif
	kako sam skontao, ako je main potreban ILI dostupan, definisi main;
	to mi nije imalo smisla, pa sam stavio negaciju kod drugog uslova, tj.
	ako NIJE DOSTUPAN, definisi main i sad taj kod izgleda ovako:
		#if defined(SDL_MAIN_NEEDED) || !defined(SDL_MAIN_AVAILABLE)
		#define main    SDL_main
		#endif
	i to je resilo problem bildovanja
*/

/*
	bitwise &
 	0xXYZABC
  & 0xFF0000
  = 0xXY0000
  (izvodio na papiru sa konverzijom u binarni sistem)
  FF0000 sluzi kao maska. FF u binarnom su sve jedinice na 8 mesta (za 1 byte);
  time se preklapa ono sto se maskira i vratice vrednosti samo na onim mestima na kojima je preklopljeno;
  s obzirom da su ostalo sve 0, na ostalim mestima ce biti 0; u primeru gore, vratice samo XY, a ostalo nije
  isunilo logicku AND operaciju;

  jednostavniji nacin da se dobije ista stvar je bithifting
  c = 0xXYZABC;
  c >> 16;
  c ce sada da bude 0xXY (0x0000XY); jer smo pomerili za 2 bajta;
  s obzirom da u hexadec treba 2 mesta za 1 bajt, ovo ga pomeri za 4 mesta i taman dodje na kraj,
  a ono sto je bilo na kraju se odbacuje
*/

int main()
{
	// da rand() ne bi svakim pokretanjem programa vracao istu pseudo slucaju sekvencu,
	// srand se seeduje nekim brojem, i u ovom slucaju seedujemo ga vremenom
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

		// sinusoidalno dobijaj nijanse za R, G, B
		unsigned char Red = (sin(TimeElapsed * 0.0005) + 1) / 2 * 255;
		unsigned char Green = (sin(TimeElapsed * 0.001) + 1) / 2 * 255;
		unsigned char Blue = (sin(TimeElapsed * 0.0015) + 1) / 2 * 255;

		// getuj svih NPARTICLES cestica, svakoj postavi neku lokaciju na Screen-u,
		// cija inicijalizacija definisana u konstruktoru Particle();
		// dodajemo 1 da bi opseg presao sa -1:1 na 0:2, pa mnozimo sa polovinom visine/sirine;
		const Particle * const pParticle = Swarm.GetParticles();
		for (size_t i(0); i < Swarm::NPARTICLES; ++i)
		{
			Particle Particle = pParticle[i];
			// i x i y mnozimo sa sirinom prozora, da bi kretanje obrazovalo
			// kruzni oblik, a ne ovalni
			int x = (Particle.Particle_X + 1) * ScreenHalfWidth;
			int y = Particle.Particle_Y * ScreenHalfWidth + ScreenHalfHeight;

			Screen.SetPixel(x, y, Red, Green, Blue);
		}

		Screen.BoxBlur();

		// refresh ekrana
		Screen.Update();
		
		// if ProcessEvent returns false (the user presses X), break the main loop
		if (!Screen.ProcessEvent())
			break;
		
	}

	// clear all pointers
	Screen.Clear();
	


	return 0;
}