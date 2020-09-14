#pragma once

//#ifndef SCREEN_H_
//#define SCREEN_H_
//#endif

#include <SDL.h>

class Screen {
	
public:
	const static int WINDOW_WIDTH = 800;
	const static int WINDOW_HEIGHT = 600;

private:
	SDL_Window *Screen_Window;
	SDL_Renderer *Screen_Renderer;
	SDL_Texture *Screen_Texture;

	// every pixel has 4 channels (red, green, blue, alpha),
	// 8 bits each, so 32 bits are needed for storing one pixel
	Uint32 *_Buffer1;
	// second buffer, for blurring
	Uint32 *_Buffer2;

public:
	Screen();
	bool Init();
	bool ProcessEvent();
	void Clear();
	void SetPixel(int x, int y, Uint8 Red, Uint8 Green, Uint8 Blue);
	void Update();
	void BoxBlur();
};