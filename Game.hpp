#include<string.h>

#include "Collider.hpp"
#include "SDL_Utils.hpp"
#include "DEFINED_VALUES.h"

#define BORDER_SIZE 16
#define INFO_BAR_SIZE 48
#define FLOOR_HEIGHT 96

#define MOVE_SPEED 500.0

#define SECOND_TO_MS 1000.0
#define REFRESH_PER_SECOND 2.0

class Game {
public:
	Game(SDL_Surface *screen, SDL_Texture *scrtex, SDL_Renderer *renderer) {
		width = SCREEN_WIDTH - 2 * BORDER_SIZE;
		height = SCREEN_HEIGHT - BORDER_SIZE - INFO_BAR_SIZE;

		this->screen = screen;
		this->scrtex = scrtex;
		this->renderer = renderer;

		charset = SDL_LoadBMP("./bmp/cs8x8.bmp");
		platform = SDL_LoadBMP("./bmp/platform16x16.bmp");
		ladder = SDL_LoadBMP("./bmp/ladder16x16.bmp");
		character = SDL_LoadBMP("./bmp/player32x48.bmp");

		black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
		green = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
		red = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
		blue = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
		currentLadder = nullptr;
		currentFloor = nullptr;
	}

	~Game() {
		currentLadder = nullptr;
		currentFloor = nullptr;
		delete(charset);
		delete(platform);
		delete(ladder);
		delete(character);
		delete(player);
		delete(floors);
		delete(ladders);
	}

	bool gameInit() {
		startSetup();
		return validateSetup();
	}

	void start() {
		SDL_Event event;
		levelSetup();

		/*player->printObject();
		floors[0]->printObject();
		floors[1]->printObject();
		ladders[0]->printObject();*/

		while (!quit) {
			workWithDelta();
			SDL_FillRect(screen, NULL, black);
			printObjects();
			printBorder();
			printInformation();
			sdl_refresh();

			setCurrentColliders();

			SDL_PollEvent(&event);
			handleEvents(event);

			frames++;
		}
	}

private:
	// SDL types
	SDL_Surface *screen, *charset, *platform, *ladder, *character;
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

	bool validateSetup() {
		if (character == NULL) {
			printf("SDL_LoadBMP character error: %s\n", SDL_GetError());
			return false;
		}

		if (ladder == NULL) {
			printf("SDL_LoadBMP ladder error: %s\n", SDL_GetError());
			return false;
		}

		if (platform == NULL) {
			printf("SDL_LoadBMP platform error: %s\n", SDL_GetError());
			return false;
		}

		if (charset == NULL) {
			printf("SDL_LoadBMP charset error: %s\n", SDL_GetError());
			return false;
		}

		SDL_SetColorKey(charset, true, 0x000000);

		return true;
	}

	void startSetup() {
		tick1 = SDL_GetTicks();
		tick2 = 0;
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

		if (fpsTimer > 1 / REFRESH_PER_SECOND) {
			fps = frames * REFRESH_PER_SECOND;
			frames = 0;
			fpsTimer -= 1 / REFRESH_PER_SECOND;
		};
	}

	void printObjects() {
		for (GameObject* go : ladders) {
			if (go == nullptr)
				break;
			go->printOnScreen(screen);
		}
		
		for (GameObject* go : floors) {
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
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				quit = 1;
			}
			else if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN) {
				if (currentLadder != nullptr && Collider::IsMovePossibleOnAxisY(*currentLadder, *player))
					player->move(event.key.keysym.sym, distance, *currentLadder);
			}
			else if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT) {
				if (Collider::IsMovePossibleOnAxisX(*currentFloor, *player))
					player->move(event.key.keysym.sym, distance, *currentFloor);
			}
			break;
		case SDL_QUIT:
			quit = 1;
			break;
		}
	}

	void levelSetup() {
		int end = SCREEN_HEIGHT - BORDER_SIZE;

		player = new Movable(character, "Player", 
				/*(SCREEN_WIDTH - character->w) / 2, end - character->h - platform->h*/
				BORDER_SIZE + 32, end - platform->h - FLOOR_HEIGHT - character->h);
		floors[0] = new GameObject(platform, "Main Floor",
			BORDER_SIZE, end - platform->h, width, platform->h);
		ladders[0] = new GameObject(ladder, "Ladder 1", 
			BORDER_SIZE + 48, end - platform->h - FLOOR_HEIGHT, ladder->w, FLOOR_HEIGHT);
		
		floors[1] = new GameObject(platform, "Floor 1",
			BORDER_SIZE + 32, end - platform->h - FLOOR_HEIGHT, width / 2, platform->h);
		//ladders[1] = new GameObject(ladder, "Ladder 1",
			//BORDER_SIZE + 32, end - platform->h - FLOOR_HEIGHT, ladder->w, FLOOR_HEIGHT);

		ladders[1] = nullptr;
		floors[2] = nullptr;
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

	void printInformation() {
		char text[128];
		sprintf(text, "\032 - Move Left \030 - Climb up \033 - Move Right \031 - Climb down");
		SDL_Utils::DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 12, text, charset);
		sprintf(text, "In game time = %.1lf s, %.0lf fps", gameTime, fps);
		SDL_Utils::DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 28, text, charset);
	}

	void setCurrentColliders() {
		currentFloor = Collider::SetCurrentCollider(floors, player, FLOOR);
		
		if (currentFloor == nullptr)
			currentFloor = floors[0];

		currentLadder = Collider::SetCurrentCollider(ladders, player, LADDER);
	}
};