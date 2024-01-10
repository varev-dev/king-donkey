#include<string.h>

#include "Collider.hpp"
#include "SDL_Utils.hpp"
#include "DEFINED_VALUES.h"

#define MOVE_SPEED 200.0
#define CLIMB_SPEED 150.0
#define JUMP_SPEED 200.0

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
		sand = SDL_LoadBMP("./bmp/sand16x16.bmp");
		wood = SDL_LoadBMP("./bmp/wood16x16.bmp");

		black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
		green = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
		red = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
		blue = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
		currentLadder = nullptr;
		currentFloor = nullptr;

		startSetup();
	}

	~Game() {
		currentLadder = nullptr;
		currentFloor = nullptr;
		
		for (int i = 0; i < MAX_FLOORS; i++) {
			if (floors[i] == nullptr)
				break;
			delete(floors[i]);
		}

		for (int i = 0; i < MAX_LADDERS; i++) {
			if (ladders[i] == nullptr)
				break;
			delete(ladders[i]);
		}

		SDL_RenderClear(renderer);
		delete(player);
	}

	bool init() {
		return validateSetup();
	}

	void start() {
		SDL_Event event;
		levelSetup();

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
			movement();

			frames++;
		}
	}

private:
	// SDL types
	SDL_Surface *screen, *charset, *platform, *ladder, *character, *sand, *wood;
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

		player->printOnScreen(screen);

		for (GameObject* go : floors) {
			if (go == nullptr)
				break;
			go->printOnScreen(screen);
		}
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
			else if ((event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN ||
					event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT) && 
					player->getState() == DEFAULT_STATE) {
				player->setDirection(event.key.keysym.sym, PRESSED);
			} else if (event.key.keysym.sym == SDLK_SPACE && 
					player->getState() == DEFAULT_STATE && player->getFloor() != nullptr) {
				player->setJumpDirection(player->getDirection(X_AXIS));
				player->setState(JUMP_STATE);
			} else if (event.key.keysym.sym == SDLK_n) {
				levelSetup();
				gameTime = 0;
			}
			break;
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN ||
					event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT)
				player->setDirection(event.key.keysym.sym, RELEASED);
			break;
		case SDL_QUIT:
			quit = 1;
			break;
		}
	}

	void movement() {
		if (player->getState() == JUMP_STATE) {
			// JUMP
			player->jump(delta * JUMP_SPEED, player->getJumpPosition());
		} else if (player->getState() == FALL_STATE) {
			// FALL
			player->fall(delta * JUMP_SPEED * 2/3, Collider::GetNearestFloor(floors, player));
		} else if (player->getState() == DEFAULT_STATE) {
			// CLIMB
			if (player->getLadder() != nullptr && Collider::IsMovePossibleOnAxisY(*player->getLadder(), *player))
				player->move(player->getDirection(Y_AXIS), delta * CLIMB_SPEED);
			// WALK
			if (Collider::IsMovePossibleOnAxisX(*player->getFloor(), *player))
				player->move(player->getDirection(X_AXIS), delta * MOVE_SPEED);
		}
	}

	void levelSetup() {
		player = new Movable(character, 
				(SCREEN_WIDTH - character->w) / 2, GAME_END_Y - character->h - platform->h);
		floors[0] = new GameObject(sand,
			BORDER_SIZE, GAME_END_Y - platform->h, 
			SCREEN_WIDTH, platform->h);
		
		floors[1] = new GameObject(wood,
			GAME_BEG_X + 48, GAME_END_Y - platform->h - FLOOR_HEIGHT, 
			128, platform->h);

		floors[2] = new GameObject(wood,
			floors[1]->getEndAxisX() + 48, GAME_END_Y - platform->h - FLOOR_HEIGHT,
			128, platform->h);

		floors[3] = new GameObject(wood,
			floors[2]->getEndAxisX() + 48, GAME_END_Y - platform->h - FLOOR_HEIGHT,
			128, platform->h);

		floors[4] = new GameObject(wood,
			floors[1]->getBeginningAxisX() + 64, GAME_END_Y - platform->h - 2 * FLOOR_HEIGHT,
			128*3/2, platform->h);

		floors[5] = new GameObject(wood,
			floors[1]->getEndAxisX() + 48, GAME_END_Y - platform->h - 3 * FLOOR_HEIGHT,
			128 * 2, platform->h);

		ladders[0] = new GameObject(ladder,
			floors[1]->getBeginningAxisX() + 32, GAME_END_Y - platform->h - FLOOR_HEIGHT,
			ladder->w, FLOOR_HEIGHT);

		ladders[1] = new GameObject(ladder,
			floors[2]->getBeginningAxisX() + 48, GAME_END_Y - platform->h - FLOOR_HEIGHT,
			ladder->w, FLOOR_HEIGHT);

		ladders[2] = new GameObject(ladder,
			floors[3]->getBeginningAxisX() + 64, floors[3]->getBeginningAxisY(),
			ladder->w, FLOOR_HEIGHT);

		ladders[3] = new GameObject(ladder,
			floors[3]->getBeginningAxisX() + 16, floors[5]->getBeginningAxisY(),
			ladder->w, FLOOR_HEIGHT * 2);

		ladders[4] = new GameObject(ladder,
			floors[5]->getBeginningAxisX() + 32, floors[5]->getBeginningAxisY(),
			ladder->w, FLOOR_HEIGHT);

		ladders[5] = new GameObject(ladder,
			floors[1]->getEndAxisX() - 32, floors[1]->getBeginningAxisY() - FLOOR_HEIGHT,
			ladder->w, FLOOR_HEIGHT);

		ladders[6] = nullptr;
		floors[6] = nullptr;
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
		player->setFloor(Collider::GetCollider(floors, player, FLOOR));
		
		if (player->getFloor() == nullptr)
			player->setFloor(floors[0]);

		player->setLadder(Collider::GetCollider(ladders, player, LADDER));
	}
};