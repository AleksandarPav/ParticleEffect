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

	// create window
	Screen_Window = SDL_CreateWindow("ParticleFireExplosion", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN_DESKTOP);
	/*Screen_Window = SDL_CreateWindow("ParticleEffectSimulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_ALLOW_HIGHDPI);*/

	// if Window isn't valid ptr
	if (Screen_Window == NULL)
	{
		SDL_Quit();
		return false;
	}

	// receives Window, index that, when is -1, means taking the first available,
	// and third argument means that refreshing of the screen should be synced with rendering
	Screen_Renderer = SDL_CreateRenderer(Screen_Window, -1, SDL_RENDERER_PRESENTVSYNC);

	// receives Renderer, pixel format (RGBA8888 means that each image channel uses 8 bits,
	// i.e. 1 byte, and so as Alpha, representing transparency), acceess defining nature
	// of the change (STATIC means changes rarely, not lockable), width and height of the window
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

	// each pixel demands 32 bits; buffer for all pixels of window size
	_Buffer1 = new Uint32[WINDOW_WIDTH * WINDOW_HEIGHT];
	_Buffer2 = new Uint32[WINDOW_WIDTH * WINDOW_HEIGHT];

	// memset enables seting up a block of memory;
	// receives a data container, value for all chanells of a pixel and buffer size
	memset(_Buffer1, 0, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(Uint32));
	memset(_Buffer2, 0, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(Uint32));

	// each byte in a buffer has a value FF, white (15*16^0 + 15*16^1 = 255)
	//Buffer[30000] = 0xFFFFFFFF;	

	return true;
}

bool Screen::ProcessEvent()
{
	SDL_Event Event;
	// while there is an event for processing, check if the event is a click for QUIT;
		// if it is, set bool to true which will cause exiting while loop
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
	// receives texture, NULL if whole thing should be updated,
	// pixel data and pitch - how many bytes between rows of pixel data
	SDL_UpdateTexture(Screen_Texture, NULL, _Buffer1, WINDOW_WIDTH * sizeof(Uint32));
	SDL_RenderClear(Screen_Renderer);
	// receives renderer, texture, nulls as indication entire texture and entire renderer are used
	SDL_RenderCopy(Screen_Renderer, Screen_Texture, NULL, NULL);
	SDL_RenderPresent(Screen_Renderer);
}

void Screen::SetPixel(int x, int y, Uint8 Red, Uint8 Green, Uint8 Blue)
{
	// if pixel goes out of the window
	if (x < 0 || x >= WINDOW_WIDTH || y < 0 || y >= WINDOW_HEIGHT)
		return;


	Uint32 Color = 0;

	// 8 bits, i.e. 256 values, in hexadec notation takes
	// 2 places (16*16 = 256); by shifting left for 8 bits, current value of
	// Color is shifted 2 places left, and in the last 2 places
	// a new color is added; so the result is RedGreenBlueAlpha, and
	// every value takes 2 places in hexadec notation;
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
	// now buffer2 contains pixels, and buffer1 is empty, it will be written to

	// go through each pixel
	for (size_t y(0); y < WINDOW_HEIGHT; ++y)
	{
		for (size_t x(0); x < WINDOW_WIDTH; ++x)
		{
			int RedTotal(0), GreenTotal(0), BlueTotal(0);

			// averaging (box filter) is implemented in 3x3 kernels around the current pixel
			for (int row(-1); row <= 1; ++row)
			{
				for (int col(-1); col <= 1; ++col)
				{
					size_t CurrentX = x + col;
					size_t CurrentY = y + row;

					// checking if it is within the window
					if (CurrentX >= 0 && CurrentX < WINDOW_WIDTH &&	CurrentY >= 0 && CurrentY < WINDOW_HEIGHT)
					{
						// color in the current position
						Uint32 Color = _Buffer2[CurrentY * WINDOW_WIDTH + CurrentX];

						// each component is shifted for the right amount of bits so that it comes to the end
						Uint8 Red = Color >> 24;
						Uint8 Green = Color >> 16;
						Uint8 Blue = Color >> 8;

						// to each component is added value from 3x3 kernel
						RedTotal += Red;
						GreenTotal += Green;
						BlueTotal += Blue;
					}
				}
			}
			// average value of every color of center pixel in 3x3 kernel
			Uint8 Red = RedTotal / 9;
			Uint8 Green = GreenTotal / 9;
			Uint8 Blue = BlueTotal / 9;

			SetPixel(x, y, Red, Green, Blue);
		}
	}
}