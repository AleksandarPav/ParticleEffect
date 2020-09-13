#include "Screen.h"
#include <iostream>
using namespace std;

Screen::Screen() : Screen_Window(NULL), Screen_Renderer(NULL), Screen_Texture(NULL), _Buffer1(NULL), _Buffer2(NULL)
{

}

bool Screen::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "SDL Init failed!" << endl;
		return false;
	}

	// kreiraj prozor
	Screen_Window = SDL_CreateWindow("ParticleFireExplosion", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN_DESKTOP);
	/*Screen_Window = SDL_CreateWindow("ParticleEffectSimulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_ALLOW_HIGHDPI);*/

	// ako Window nije uspeo da bude validan ptr
	if (Screen_Window == NULL)
	{
		SDL_Quit();
		return false;
	}

	// prima Window, index koji kad je -1 znaci uzmi prvi slobodan,
	// a treci argument znaci da refresovanje ekrana sinhronizuje s renderovanjem
	// da se ne bi dobijale neke gluposti na ekranu
	Screen_Renderer = SDL_CreateRenderer(Screen_Window, -1, SDL_RENDERER_PRESENTVSYNC);

	// prima Renderer, pixel format (RGBA8888 znaci za svaki kanal slike koristi 8 bitova,
	// odnosno 1 byte, kao i za Alpha, koji predstavlja transparency), acceess koji definise prirodu
	// promene (STATIC znaci changes rarely, not lockable), sirinu i visinu prozora
	Screen_Texture = SDL_CreateTexture(Screen_Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, WINDOW_WIDTH, WINDOW_HEIGHT);

	if (Screen_Renderer == NULL)
	{
		cout << "Could not create renderer!" << endl;
		SDL_DestroyWindow(Screen_Window);
		SDL_Quit();
		return false;
	}

	if (Screen_Texture == NULL)
	{
		cout << "Could not create texture!" << endl;
		SDL_DestroyRenderer(Screen_Renderer);
		SDL_DestroyWindow(Screen_Window);
		SDL_Quit();
		return false;
	}

	// svaki piksel zahteva 32 bita; bafer za sve piksele u velicini prozora
	_Buffer1 = new Uint32[WINDOW_WIDTH * WINDOW_HEIGHT];
	_Buffer2 = new Uint32[WINDOW_WIDTH * WINDOW_HEIGHT];

	// memset omogucava da se setuje blok memorije, odredjenom vrednoscu;
	// prima u sta da trpa podatke, vrednost za sve kanale jednog piksela i velicinu bafera
	memset(_Buffer1, 0, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(Uint32));
	memset(_Buffer2, 0, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(Uint32));

	// svaki bajt koji se nalazi na jednom mestu u baferu dobio vrednost FF, tj. belo (15*16^0 + 15*16^1 = 255)
	//Buffer[30000] = 0xFFFFFFFF;

	

	return true;
}

bool Screen::ProcessEvent()
{
	SDL_Event Event;
	// dok postoji event za obradjivanje, proveri da li je event klik za QUIT;
		// ako jeste, postavi bool na true sto ce uzrokovati izlazak iz while loopa
	while (SDL_PollEvent(&Event))
		if (Event.type == SDL_QUIT)
			return false;

	return true;
}

void Screen::Clear()
{
	delete[] _Buffer1;
	delete[] _Buffer2;
	SDL_DestroyRenderer(Screen_Renderer);
	SDL_DestroyTexture(Screen_Texture);
	SDL_DestroyWindow(Screen_Window);
	SDL_Quit();
}

void Screen::Update()
{
	// prima teksturu, NULL ako hocemo da se updateuje the whole thing,
	// pixel data i pitch - koliko bajtova da izmedju redova piksela da bude
	SDL_UpdateTexture(Screen_Texture, NULL, _Buffer1, WINDOW_WIDTH * sizeof(Uint32));
	SDL_RenderClear(Screen_Renderer);
	// prima renderer, texture, null-ove kao indikaciju da se koristi entire texture i entire renderer
	SDL_RenderCopy(Screen_Renderer, Screen_Texture, NULL, NULL);
	SDL_RenderPresent(Screen_Renderer);
}

void Screen::SetPixel(int x, int y, Uint8 Red, Uint8 Green, Uint8 Blue)
{
	// ako piksel izleti van prozora
	if (x < 0 || x >= WINDOW_WIDTH || y < 0 || y >= WINDOW_HEIGHT)
		return;


	Uint32 Color = 0;

	// 8 bitova, odnosno 256 vrednosti, u hexadec zapisu zauzima
	// 2 mesta (16*16 = 256); pomeranjem ulevo za 8 bitova, pomeramo
	// trenutnu vrednost Color za 2 mesta ulevo, a na poslednja 2 mesta
	// dodajemo novu boju; tako na kraju dobijemo RedGreenBlueAlpha, pri cemu
	// svaka vrednost zauzima dva mesta u hexadec zapisu;
	Color += Red;
	Color <<= 8;
	Color += Green;
	Color <<= 8;
	Color += Blue;
	Color <<= 8;
	Color += 0xFF;

	_Buffer1[y * WINDOW_WIDTH + x] = Color;
}

void Screen::BoxBlur()
{
	// swap buffers
	Uint32 *tmp = _Buffer1;
	_Buffer1 = _Buffer2;
	_Buffer2 = tmp;
	// sad buffer2 sadrzi piksele, a buffer1 prazan, u njega cemo upisati

	for (size_t y(0); y < WINDOW_HEIGHT; ++y)
	{
		for (size_t x(0); x < WINDOW_WIDTH; ++x)
		{
			int RedTotal(0), GreenTotal(0), BlueTotal(0);

			// implementiramo usrednjavanje (box filtar) u okolini 3x3
			for (int row(-1); row <= 1; ++row)
			{
				for (int col(-1); col <= 1; ++col)
				{
					size_t CurrentX = x + col;
					size_t CurrentY = y + row;

					// provera da nije izasao van okvira prozora
					if (CurrentX >= 0 && CurrentX < WINDOW_WIDTH &&	CurrentY >= 0 && CurrentY < WINDOW_HEIGHT)
					{
						// boja na trenutnoj poziciji
						Uint32 Color = _Buffer2[CurrentY * WINDOW_WIDTH + CurrentX];

						// svaka komponenta se siftuje za odgovarajuci broj bitova kako bi dosla na kraj
						Uint8 Red = Color >> 24;
						Uint8 Green = Color >> 16;
						Uint8 Blue = Color >> 8;

						// na zbir svake komponente dodaj svaku vrednost iz okoline 3x3
						RedTotal += Red;
						GreenTotal += Green;
						BlueTotal += Blue;
					}
				}
			}
			// prosecna vrednost svake boje centralnog piksela u okolini 3x3
			Uint8 Red = RedTotal / 9;
			Uint8 Green = GreenTotal / 9;
			Uint8 Blue = BlueTotal / 9;

			SetPixel(x, y, Red, Green, Blue);
		}
	}
}