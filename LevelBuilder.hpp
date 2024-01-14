#include "Collider.hpp"
#include "DEFINED_VALUES.h"

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

class Level {
public:
	SDL_Surface* s_platform, * s_ladder, * s_character, * s_sand, * s_wood, * s_rotten, * s_footbridge, * s_barrel, * s_boss;
	GameObject* floors[MAX_ELEMENTS], * ladders[MAX_ELEMENTS], * finishObject, * spawner;
	Movable* player, * barrels[MAX_ELEMENTS/2];
	int floors_ctr, ladders_ctr, barrels_ctr;
	double lastSpawnTime;

	Level() {
		s_platform = SDL_LoadBMP("./bmp/platform16x16.bmp");
		s_ladder = SDL_LoadBMP("./bmp/ladder16x16.bmp");
		s_character = SDL_LoadBMP("./bmp/player32x48.bmp");
		s_sand = SDL_LoadBMP("./bmp/sand16x16.bmp");
		s_wood = SDL_LoadBMP("./bmp/wood16x16.bmp");
		s_rotten = SDL_LoadBMP("./bmp/rotten-wood16x16.bmp");
		s_footbridge = SDL_LoadBMP("./bmp/footbridge16x16.bmp");
		s_barrel = SDL_LoadBMP("./bmp/barrel24x24.bmp");
		s_boss = SDL_LoadBMP("./bmp/boss64x64.bmp");

		resetLevel();
	}

	~Level() {
		resetLevel();
		SDL_FreeSurface(s_barrel);
		SDL_FreeSurface(s_boss);
		SDL_FreeSurface(s_character);
		SDL_FreeSurface(s_footbridge);
		SDL_FreeSurface(s_ladder);
		SDL_FreeSurface(s_platform);
		SDL_FreeSurface(s_rotten);
		SDL_FreeSurface(s_sand);
		SDL_FreeSurface(s_wood);
	}

	void setUpLevel(int level) {
		resetLevel();

		lastSpawnTime = 0;

		player = new Movable(s_character,
			(SCREEN_WIDTH - s_character->w) / 2, GAME_END_Y - s_character->h - s_platform->h);

		if (level == 1) {
			// FLOORS
			createMainFloor();
			// first lv
			addFloor(144, floors[0]->getHeight() + FLOOR_HEIGHT, GAME_END_X - GAME_BEG_X - 144, PLANKS);
			// second lv
			addFloor(0, floors[0]->getHeight() + FLOOR_HEIGHT * 2, GAME_END_X - GAME_BEG_X - 144, PLANKS);
			addFloor(GAME_END_X - 72, floors[0]->getHeight() + FLOOR_HEIGHT * 2, 72, ROTTEN);
			// third lv
			addFloor(0, floors[0]->getHeight() + FLOOR_HEIGHT * 3, GAME_END_X - GAME_BEG_X, PLANKS);

			// LADDERS
			addLadder(144 + 32, floors[1]->getBeginningAxisY(), 1, 1);
			addLadder(floors[2]->getEndAxisX() - 32, floors[2]->getBeginningAxisY(), 1, 1);
			addLadder(floors[4]->getBeginningAxisX() + 64, floors[4]->getBeginningAxisY(), 1, 1);
			addLadder(floors[4]->getBeginningAxisX() + 256, floors[4]->getBeginningAxisY(), 1, 1);

			spawner = new GameObject(s_boss, GAME_END_X - 72, floors[4]->getBeginningAxisY() - 64);
			addLadder(floors[floors_ctr - 1]->getCenterOfAxisX(), floors[floors_ctr - 1]->getBeginningAxisY() - FLOOR_HEIGHT, 2, 1);
			finishObject = ladders[ladders_ctr - 1];
		} else if (level == 2) {
			// FLOORS
			// ground lv
			createMainFloor(PLANKS);

			// first lv
			addFloor(256, getFloorHeight(floors[0], 1), 96, PLANKS);
			addFloor(256 + 96, getFloorHeight(floors[0], 1), 64, FOOTBRIDGE);
			addFloor(256 + 96 + 64, getFloorHeight(floors[0], 1), 96, PLANKS);

			// second lv
			addFloor(0, getFloorHeight(floors[0], 2), 96, ROTTEN);
			addFloor(120 + JUMP_WIDTH, getFloorHeight(floors[0], 2), 160, PLANKS);
			addFloor(120 + JUMP_WIDTH + 160, getFloorHeight(floors[0], 2), 96, FOOTBRIDGE);
			addFloor(120 + JUMP_WIDTH + 160 + 96, getFloorHeight(floors[0], 2), 256, PLANKS);

			// third lv
			addFloor(48, getFloorHeight(floors[0], 3), 72, ROTTEN);
			addFloor(144 + JUMP_WIDTH * 1 / 2, getFloorHeight(floors[0], 3), 64, PLANKS);
			addFloor(144 + JUMP_WIDTH * 1 / 2 + 64, getFloorHeight(floors[0], 3), 64, FOOTBRIDGE);
			addFloor(144 + JUMP_WIDTH * 1 / 2 + 128, getFloorHeight(floors[0], 3), SCREEN_WIDTH, PLANKS);

			// LADDERS
			addLadder(floors[1]->getCenterOfAxisX() - 8, floors[1]->getBeginningAxisY(), 1, 1);
			addLadder(floors[7]->getBeginningAxisX() + 16, floors[7]->getBeginningAxisY(), 1, 1);
			addLadder(GAME_END_X - 48, floors[7]->getBeginningAxisY(), 2, 2);
			addLadder(floors[8]->getCenterOfAxisX() - 8, floors[8]->getBeginningAxisY(), 1, 1);
			addLadder(floors[11]->getBeginningAxisX() + 32, floors[11]->getBeginningAxisY(), 1, 1);

			// FINISH
			addLadder(floors[9]->getCenterOfAxisX() - 16, floors[9]->getBeginningAxisY() - FLOOR_HEIGHT, 2, 1);

			spawner = new GameObject(s_boss, GAME_END_X - 72, floors[11]->getBeginningAxisY() - 64);
			finishObject = ladders[ladders_ctr - 1];
		} else if (level == 3) {
			createMainFloor(ROTTEN);

			addFloor(48, getFloorHeight(floors[0], 1), 96, FOOTBRIDGE);
			addFloor(48+196+JUMP_WIDTH, getFloorHeight(floors[0], 1), 112, FOOTBRIDGE);
			addFloor(GAME_END_X-64-16, getFloorHeight(floors[0], 1), 64, ROTTEN);

			addFloor(0, getFloorHeight(floors[0], 2), 80, FOOTBRIDGE);
			addFloor(48 + 64, getFloorHeight(floors[0], 2), 80, FOOTBRIDGE);
			addFloor(48 + 64 + 80 + JUMP_WIDTH, getFloorHeight(floors[0], 2), 128, ROTTEN);
			addFloor(GAME_END_X-64-16, getFloorHeight(floors[0], 2), 64, ROTTEN);

			addFloor(96, getFloorHeight(floors[0], 3), GAME_END_X - 96 - 16 - 80, FOOTBRIDGE);

			addFloor(0, getFloorHeight(floors[0], 3.2), 80, SAND);

			addLadder(floors[1]->getCenterOfAxisX() - 16, floors[1]->getBeginningAxisY(), 2, 1);
			addLadder(floors[1]->getBeginningAxisX() + 8, floors[4]->getBeginningAxisY(), 1, 1);
			addLadder(floors[1]->getEndAxisX() - 24, floors[5]->getBeginningAxisY(), 1, 1);
			addLadder(floors[5]->getEndAxisX() - 24, floors[8]->getBeginningAxisY(), 1, 1);
			addLadder(floors[1]->getEndAxisX() - 24, floors[5]->getBeginningAxisY(), 1, 1);
			addLadder(floors[7]->getCenterOfAxisX() + 8, floors[7]->getBeginningAxisY(), 1, 1);
			addLadder(floors[7]->getCenterOfAxisX() - 24, floors[8]->getBeginningAxisY(), 1, 1);
			addLadder(floors[8]->getBeginningAxisX() + 128, floors[8]->getBeginningAxisY(), 2, 3);

			spawner = new GameObject(s_boss, GAME_END_X - 144, floors[8]->getBeginningAxisY() - 64);
			finishObject = floors[floors_ctr - 1];
		}
	}

	bool checkIsPlayerOnFinish() {
		return Collider::CollisionBetweenMovableAndObject(*finishObject, *player);
	}

	void updateBarrels(double currentTime) {
		for (int i = 0; i < barrels_ctr; i++) {
			if (barrels[i]->getFloor() != floors[0])
				continue;

			double finalX = FINISH_DIRECTION == LEFT ? GAME_BEG_X : GAME_END_X - s_barrel->w;

			if (FINISH_DIRECTION == LEFT && barrels[i]->getBeginningAxisX() > 0)
				continue;
			if (FINISH_DIRECTION == RIGHT && barrels[i]->getEndAxisX() < SCREEN_WIDTH)
				continue;

			removeBarrel(i);
		}

		if (lastSpawnTime + SPAWN_DELAY / 1000.0 > currentTime)
			return;

		if (barrels_ctr == MAX_BARRELS)
			return;

		createBarrel();
		lastSpawnTime = currentTime;
	}

	void createBarrel() {
		barrels[barrels_ctr++] = new Movable(s_barrel, spawner->getCenterOfAxisX(), spawner->getEndAxisY() - s_barrel->h);
	}

	void removeBarrel(int id) {
		if (id >= MAX_BARRELS || id >= barrels_ctr)
			return;

		Movable* removed = barrels[id];
		barrels[id] = nullptr;

		for (int i = id; i < barrels_ctr - 1; i++) {
			Movable* temp = barrels[i + 1];
			barrels[i + 1] = nullptr;
			barrels[i] = temp;
		}

		barrels_ctr--;
	}

	bool validateSetup() const  {
		if (s_character == NULL) {
			printf("SDL_LoadBMP character error: %s\n", SDL_GetError());
			return false;
		}

		if (s_barrel == NULL) {
			printf("SDL_LoadBMP barrel error: %s\n", SDL_GetError());
			return false;
		}

		if (s_ladder == NULL) {
			printf("SDL_LoadBMP ladder error: %s\n", SDL_GetError());
			return false;
		}

		if (s_platform == NULL) {
			printf("SDL_LoadBMP platform error: %s\n", SDL_GetError());
			return false;
		}

		if (s_sand == NULL) {
			printf("SDL_LoadBMP sand error: %s\n", SDL_GetError());
			return false;
		}

		if (s_wood == NULL) {
			printf("SDL_LoadBMP wood error: %s\n", SDL_GetError());
			return false;
		}

		if (s_rotten == NULL) {
			printf("SDL_LoadBMP rotten error: %s\n", SDL_GetError());
			return false;
		}

		if (s_footbridge == NULL) {
			printf("SDL_LoadBMP footbridge error: %s\n", SDL_GetError());
			return false;
		}

		if (s_boss == NULL) {
			printf("SDL_LoadBMP boss error: %s\n", SDL_GetError());
			return false;
		}

		return true;
	}

	void resetLevel() {
		floors_ctr = ladders_ctr = barrels_ctr = lastSpawnTime = 0;

		finishObject = nullptr;

		player = nullptr;

		for (int i = 0; i < MAX_ELEMENTS; i++) {
			floors[i] = nullptr;
		}

		for (int i = 0; i < MAX_ELEMENTS; i++) {
			ladders[i] = nullptr;
		}

		for (int i = 0; i < MAX_ELEMENTS / 2; i++) {
			barrels[i] = nullptr;
		}
	}

	void createMainFloor(int type = SAND) {
		this->floors[0] = new GameObject(s_sand,
			BORDER_SIZE, GAME_END_Y - s_platform->h,
			GAME_END_X - GAME_BEG_X, s_platform->h);

		if (type == PLANKS)
			this->floors[0]->setSprite(s_wood);
		
		if (type == ROTTEN)
			this->floors[0]->setSprite(s_rotten);

		floors_ctr = 1;
	}

	void addFloor(int x, int y, int width, int type = SAND) {
		if (floors_ctr == MAX_ELEMENTS || width > SCREEN_WIDTH)
			return;
		
		SDL_Surface *srf = nullptr;

		x += GAME_BEG_X;
		y *= -1;
		y += GAME_END_Y;

		switch (type) {
		case SAND:
			srf = s_sand;
			break;
		case PLANKS:
			srf = s_wood;
			break;
		case ROTTEN:
			srf = s_rotten;
			break;
		case FOOTBRIDGE:
			srf = s_footbridge;
			break;
		default:
			return;
		}
		
		roundWidthAndHeight(&width, 0, srf->w, srf->h);

		floors[floors_ctr++] = new GameObject(srf, x, y, width, srf->h);
	}

	void addLadder(int x, int y, int size, int floorNumber) {
		if (ladders_ctr == MAX_ELEMENTS)
			return;

		SDL_Surface* srf = s_ladder;

		int height = FLOOR_HEIGHT * floorNumber;
		int width = srf->w * size;

		roundWidthAndHeight(&width, &height, srf->w, srf->h);

		ladders[ladders_ctr++] = new GameObject(srf, x, y, width, height);
	}

	void setCurrentColliders(Movable* movable) {
		movable->setFloor(Collider::GetCollider(floors, movable, FLOOR));

		if (movable->getFloor() == nullptr)
			movable->setFloor(floors[0]);

		movable->setLadder(Collider::GetCollider(ladders, movable, LADDER));
	}

private:
	void roundWidthAndHeight(int *width, int *height, int srf_w, int srf_h) {
		if (width != nullptr && srf_w != 0 && *width % srf_w)
			*width -= *width % srf_w;
		if (height != nullptr && srf_h != 0 && *height % srf_h)
			*height -= *height % srf_h;
	}

	double getFloorHeight(GameObject* main, double floorNumber) {
		return main->getHeight() + FLOOR_HEIGHT * floorNumber;
	}
};