#include<string.h>

#include "LevelBuilder.hpp"
#include "SDL_Utils.hpp"
#include "DEFINED_VALUES.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MOVE_SPEED 200.0
#define CLIMB_SPEED 150.0
#define JUMP_SPEED 250.0
#define FALL_SPEED 300.0

#define SECOND_TO_MS 1000.0
#define REFRESH_PER_SECOND 2.0

class Game {
public:
	Game(SDL_Surface* screen, SDL_Texture* scrtex, SDL_Renderer* renderer) {
		width = SCREEN_WIDTH - 2 * BORDER_SIZE;
		height = SCREEN_HEIGHT - BORDER_SIZE - INFO_BAR_SIZE;

		this->screen = screen;
		this->scrtex = scrtex;
		this->renderer = renderer;

		charset = SDL_LoadBMP("./bmp/cs8x8.bmp");
		level = new Level();

		black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
		green = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
		red = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
		blue = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);

		startSetup();
	}

	~Game() {
		SDL_RenderClear(renderer);
		delete(level);
		SDL_FreeSurface(charset);
	}

	bool init() {
		if (charset == NULL) {
			printf("SDL_LoadBMP charset error: %s\n", SDL_GetError());
			return false;
		}

		SDL_SetColorKey(charset, true, 0x000000);

		if (!level->validateSetup())
			return false;

		return true;
	}

	void start() {
		SDL_Event event;
		level->setUpLevel(1);

		while (!quit) {
			workWithDelta();
			// render objects -> borders -> info
			SDL_FillRect(screen, NULL, black);
			printObjects();
			printBorder();
			printInformation();
			sdl_refresh();

			// player input
			SDL_PollEvent(&event);
			if (handleEvents(event))
				continue;

			if (playing) {
				level->updateBarrels(gameTime);
				level->setCurrentColliders(level->player);
				movement();

				// barrel
				iterateBarrelsToHandleEvents();
			}

			frames++;
		}
	}

private:
	// SDL types
	SDL_Surface* screen, * charset;
	SDL_Texture* scrtex;
	SDL_Renderer* renderer;
	Level* level;

	// Utils
	int tick1, tick2, quit, frames;
	double delta, gameTime, fpsTimer, fps, distance, mv_speed;
	bool playing;

	// Colors
	int black, green, red, blue;

	// Game Window
	int width, height;

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
		for (GameObject* go : level->ladders) {
			if (go == nullptr)
				break;
			go->printOnScreen(screen);
		}

		level->player->printOnScreen(screen);
		level->spawner->printOnScreen(screen);

		for (int i = 0; i < level->barrels_ctr; i++) {
			level->barrels[i]->printOnScreen(screen);
		}

		for (GameObject* go : level->floors) {
			if (go == nullptr)
				break;
			go->printOnScreen(screen);
		}
	}

	void sdl_refresh() {
		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		// SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	bool handleEvents(SDL_Event event) {
		SDL_Keycode key = event.key.keysym.sym;
		bool moveKey = (key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT || key == KEY_RIGHT);
		switch (event.type) {
		case SDL_KEYDOWN:
			if (key == KEY_ESCAPE) {
				quit = 1;
			} else if (key == KEY_CONTINUE || key == KEY_RESET) {
				playing = true;
				level->setUpLevel(1);
				gameTime = 0;
				return true;
			} else if (moveKey && level->player->getState() == DEFAULT_STATE) {
				level->player->setDirection(key, PRESSED);
			} else if (key == KEY_JUMP && level->player->getState() == DEFAULT_STATE && level->player->getFloor() != nullptr) {
				level->player->setJumpDirection(level->player->getDirection(X_AXIS));
				level->player->setState(JUMP_STATE);
			}
			break;
		case SDL_KEYUP:
			if (moveKey)
				level->player->setDirection(key, RELEASED);
			break;
		case SDL_QUIT:
			quit = 1;
			break;
		}
		return false;
	}

	void movement() {
		Movable* player = level->player;
		if (player->getState() == JUMP_STATE) {
			// JUMP
			player->setFloor(Collider::GetNearestFloor(level->floors, player));
			player->jump(delta * JUMP_SPEED, player->getJumpPosition());
		} else if (player->getState() == FALL_STATE) {
			// FALL
			player->fall(delta * FALL_SPEED, Collider::GetNearestFloor(level->floors, player));
		} else if (player->getState() == DEFAULT_STATE) {
			// CLIMB
			if (player->getLadder() != nullptr && Collider::IsMovePossibleOnAxisY(*player->getLadder(), *player))
				player->move(player->getDirection(Y_AXIS), delta * CLIMB_SPEED);
			// WALK
			if (Collider::IsMovePossibleOnAxisX(*player->getFloor(), *player))
				player->move(player->getDirection(X_AXIS), delta * MOVE_SPEED);
		}
	}

	void iterateBarrelsToHandleEvents() {
		for (int i = 0; i < level->barrels_ctr; i++) {
			level->setCurrentColliders(level->barrels[i]);
			barrelEvent(level->barrels[i]);
			movement(level->barrels[i], 1, 0);
			if (Collider::CollisionBetweenMovables(level->player, level->barrels[i]))
				playing = false;
		}
	}

	void barrelEvent(Movable* barrel) {
		if (barrel->getFloor() != nullptr && barrel->getFloor()->getBeginningAxisY() != barrel->getEndAxisY() && barrel->getDirection(Y_AXIS) == UNDEF) {
			barrel->setState(FALL_STATE);
			return;
		}

		if (barrel->getEndAxisY() == level->floors[0]->getBeginningAxisY()) {
			setBarrelDirectionOnX(barrel);
			barrel->setState(DEFAULT_STATE);
			return;
		}

		int random = rand() % 2 == 1;

		bool floorAvailable = barrel->getFloor() != nullptr && barrel->getEndAxisY() == barrel->getFloor()->getBeginningAxisY(), 
			ladderAvailable = ALLOW_CLIMB_DOWN && barrel->getLadder() != nullptr && level->finishObject != barrel->getLadder() && barrel->getEndAxisY() == barrel->getLadder()->getBeginningAxisY();
		bool endOfFloor = 0, endOfFloorIsEndOfView, endOfLadder = 0;

		if (floorAvailable) {
			endOfFloor = barrel->getBeginningAxisX() - distance <= fmax(barrel->getFloor()->getBeginningAxisX(), GAME_BEG_X) ||
						barrel->getEndAxisX() + distance >= fmin(barrel->getFloor()->getEndAxisX(), GAME_END_X);
			endOfFloorIsEndOfView = barrel->getFloor()->getEndAxisX() >= GAME_END_X || barrel->getFloor()->getBeginningAxisX() <= GAME_BEG_X;
		}


		if (endOfFloor) {
			int mode = barrel->getBeginningAxisX() - distance <= fmax(barrel->getFloor()->getBeginningAxisX(), GAME_BEG_X) ? BEG_MODE : END_MODE;

			if (!Collider::IsFloorExtended(level->floors, barrel->getFloor(), mode) && (!ALLOW_FALL || endOfFloorIsEndOfView)) {
				barrel->switchDirection(X_AXIS);
			}

		}
		else if (barrel->getDirection(X_AXIS) == UNDEF) {
			setBarrelDirectionOnX(barrel);
			barrel->setState(DEFAULT_STATE);
		}

		if (ladderAvailable && 
				barrel->getCenterOfAxisX() < barrel->getLadder()->getCenterOfAxisX() + distance && 
				barrel->getCenterOfAxisX() > barrel->getLadder()->getCenterOfAxisX() - distance)
			endOfLadder = barrel->getEndAxisY() == barrel->getLadder()->getBeginningAxisY();

		if (barrel->getLadder() != nullptr && barrel->getEndAxisY() == barrel->getLadder()->getEndAxisY()) {
			barrel->setDirection(barrel->getDirection(Y_AXIS), RELEASED);
		}

		if (endOfLadder && random)
			setBarrelDirectionOnY(barrel);
	}

	void setBarrelDirectionOnX(Movable* mv) {
		int random = rand() % 2;
		if (mv->getDirection(X_AXIS) == UNDEF) {
			if (FORCE_FINISH && mv->getFloor() == level->floors[0]) {
				mv->setDirection(FINISH_DIRECTION, PRESSED);
			} else {
				mv->setDirection(random ? LEFT : RIGHT, PRESSED);
			}
		}
	}

	void setBarrelDirectionOnY(Movable* mv) {
		int random = rand() % 2;

		if (ALWAYS_DOWN) {
			mv->setDirection(DOWN, PRESSED);
		} else {
			mv->setDirection(random ? UP : DOWN, PRESSED);
		}

		mv->setDirection(mv->getDirection(X_AXIS), RELEASED);
	}

	void movement(Movable* mv, double speed = 1, int limit = 1) {
		if (mv->getState() == JUMP_STATE) {
			// JUMP
			mv->setFloor(Collider::GetNearestFloor(level->floors, mv));
			mv->jump(delta * speed * JUMP_SPEED, mv->getJumpPosition(), limit);
		}
		else if (mv->getState() == FALL_STATE) {
			// FALL
			mv->fall(delta * speed * FALL_SPEED, Collider::GetNearestFloor(level->floors, mv), limit);
		}
		else if (mv->getState() == DEFAULT_STATE) {
			// CLIMB
			if (mv->getLadder() != nullptr && Collider::IsMovePossibleOnAxisY(*mv->getLadder(), *mv))
				mv->move(mv->getDirection(Y_AXIS), delta * speed * CLIMB_SPEED, limit);
			// WALK
			if (Collider::IsMovePossibleOnAxisX(*mv->getFloor(), *mv))
				mv->move(mv->getDirection(X_AXIS), delta * speed * MOVE_SPEED, limit);
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

	void printInformation() {
		char text[128];
		sprintf(text, "Donkey Kong");
		SDL_Utils::DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 12, text, charset);
		sprintf(text, "In game time = %.1lf s, %.0lf fps", gameTime, fps);
		SDL_Utils::DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 28, text, charset);
	}

};