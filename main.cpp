#define _USE_MATH_DEFINES
#include<stdio.h>
#include<string.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#include "DEFINED_VALUES.h"
#include "Game.hpp";

// main
#ifdef __cplusplus
extern "C"
#endif

void clearAll(SDL_Surface *screen, SDL_Texture *scrtex, SDL_Window *window, SDL_Renderer *renderer) {
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

int main(int argc, char **argv) {
	int rc;
	SDL_Surface* screen;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;

	// Validate SDL init
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	// Set up window & renderer
	//rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer);
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);

	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	};
	
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "King Donkey");

	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
	                              0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
	                           SCREEN_WIDTH, SCREEN_HEIGHT);

	// wy³¹czenie widocznoœci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);

	Game *game = new Game(screen, scrtex, renderer);
	// validate game init
	if (game->init())
		game->start();

//		if (!Collider::CollisionBetweenMovableAndObject(*floor, *player) &&
//			!Collider::CollisionBetweenMovableAndObject(*ladder, *player)) {
//			player->move(SDLK_DOWN, 2, *ladder);
//		}

	// free memory
	delete(game);
	clearAll(screen, scrtex, window, renderer);
	return 0;
};
