#include "Game.hpp"

Game::Game(SDL_Surface* screen, SDL_Texture* scrtex, SDL_Renderer* renderer) {
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

Game::~Game() {
	SDL_RenderClear(renderer);
	delete(level);
	SDL_FreeSurface(charset);
}

bool Game::init() {
	if (charset == NULL) {
		printf("SDL_LoadBMP charset error: %s\n", SDL_GetError());
		return false;
	}

	SDL_SetColorKey(charset, true, 0x000000);

	if (!level->validateSetup())
		return false;

	return true;
}

void Game::start() {
	SDL_Event event;
	currentLevel = 1;
	level->setUpLevel(currentLevel);

	while (!quit) {
		// render objects -> borders -> info
		SDL_FillRect(screen, NULL, black);

		if (currentLevel <= MAX_LEVEL) {
			printObjects();
			workWithDelta();
		} else {
			printWon();
			playing = 0;
		}

		printBorder();
		printInformation();
		sdl_refresh();

		// player input
		SDL_PollEvent(&event);
		if (handleEvents(event))
			continue;

		if (playing) {
			if (level->checkIsPlayerOnFinish()) {
				currentLevel <= MAX_LEVEL ? currentLevel++ : 0;
				level->setUpLevel(currentLevel);
				sdl_refresh();
				continue;
			}

			level->updateBarrels(gameTime);
			level->setCurrentColliders(level->player);
			movement();

			// barrel
			iterateBarrelsToHandleEvents();
		}

		frames++;
	}
}