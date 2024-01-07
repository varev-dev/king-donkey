#include "Collider.hpp"
#include "SDL_Utils.hpp"
#include "DEFINED_VALUES.h"

#define MAX_LADDERS 8
#define MAX_FLOORS 4

#define FLOOR_HEIGHT 96

#define MOVE_SPEED 400

#define SECOND_TO_MS 1000
#define REFRESH_PER_SECOND 2
#define BORDER_SIZE 16
#define INFO_BAR_SIZE 48

class Game {
public:
	Game(SDL_Surface *screen, SDL_Texture *scrtex, SDL_Renderer *renderer) {
		width = SCREEN_WIDTH - 2 * BORDER_SIZE;
		height = SCREEN_HEIGHT - BORDER_SIZE - INFO_BAR_SIZE;

		this->screen = screen;
		this->scrtex = scrtex;
		this->renderer = renderer;

		/*charset = SDL_LoadBMP("./bmp/cs8x8.bmp");
		platform = SDL_LoadBMP("./bmp/platform16x16.bmp");
		ladder = SDL_LoadBMP("./bmp/ladder16x16.bmp");
		character = SDL_LoadBMP("./bmp/player32x48.bmp");*/

		black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
		green = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
		red = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
		blue = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);

		/*charset = nullptr;
		platform = nullptr; 
		ladder = nullptr; 
		character = nullptr; 
		player = nullptr;
		currentLadder = nullptr; 
		currentFloor = nullptr;*/
		startSetup();
	}
	~Game() {
		/*delete(charset);
		delete(platform);
		delete(ladder);
		delete(character);
		delete(player);
		delete(currentLadder);
		delete(currentFloor);*/
	}

	bool gameInit() {
		startSetup();
	}

	void start() {
		SDL_Event event;

		while (!quit) {
			workWithDelta();
			SDL_FillRect(screen, NULL, black);
			printBorder();
			sdl_refresh();
			//printObjects();

			while (SDL_PollEvent(&event)) {
				handleEvents(event);
			}
		}
	}

private:
	// SDL types
	SDL_Surface *screen, *charset, * platform, * ladder, * character;
	SDL_Texture *scrtex;
	SDL_Renderer *renderer;

	// Utils
	int tick1, tick2, quit, frames;
	double delta, gameTime, fpsTimer, fps, distance, mv_speed;

	// Colors
	int black, green, red, blue;

	// Game Window
	int width, height;

	// All Game Objects
	GameObject *ladders[MAX_LADDERS], *floors[MAX_FLOORS];
	Movable *player;

	// Objects currently interacting with player
	GameObject *currentLadder, *currentFloor;

	void startSetup() {
		tick1 = SDL_GetTicks();
		frames = 0;
		fpsTimer = 0;
		fps = 0;
		quit = 0;
		gameTime = 0;
		distance = 0;
		mv_speed = MOVE_SPEED;
	}

	void workWithDelta() {
		tick2 = SDL_GetTicks();
		delta = (tick2 - tick1) / SECOND_TO_MS;
		tick1 = tick2;
		gameTime += delta;
		distance = mv_speed * delta;

		fpsTimer += delta;

		if (fpsTimer > 1.0 / REFRESH_PER_SECOND) {
			fps = frames * REFRESH_PER_SECOND;
			frames = 0;
			fpsTimer -= 1.0 / REFRESH_PER_SECOND;
		};
	}

	void printObjects() {
		for (GameObject *go : floors) {
			if (go == nullptr)
				break;

			go->printOnScreen(screen);
		}

		for (GameObject* go : ladders) {
			if (go == nullptr)
				break;

			go->printOnScreen(screen);
		}

		player->printOnScreen(screen);
	}

	void sdl_refresh() {
		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		// SDL_RenderClear(renderer); (?)
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	void handleEvents(SDL_Event event) {
		switch (event.type) {
		case SDL_QUIT:
			quit = 1;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				quit = 1;
			break;
		}
	}

	void printBorder() {
		// Top border
		SDL_Utils::DrawRectangle(screen, 0, 0, 
			SCREEN_WIDTH, INFO_BAR_SIZE, 
			blue, blue);
		// Left border
		SDL_Utils::DrawRectangle(screen, 0, INFO_BAR_SIZE, 
			BORDER_SIZE, SCREEN_HEIGHT - INFO_BAR_SIZE, 
			blue, blue);
		// Right border
		SDL_Utils::DrawRectangle(screen, SCREEN_WIDTH - BORDER_SIZE, INFO_BAR_SIZE, 
			BORDER_SIZE, SCREEN_HEIGHT - INFO_BAR_SIZE, 
			blue, blue);
		// Bottom border
		SDL_Utils::DrawRectangle(screen, 0, SCREEN_HEIGHT - BORDER_SIZE, 
			SCREEN_WIDTH, BORDER_SIZE, blue, blue);
	}

};