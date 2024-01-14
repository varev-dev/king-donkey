#pragma once
extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

class SDL_Utils {
public:
	static void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset);

	static void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);

	static void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);

	static void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);

	static void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);
};