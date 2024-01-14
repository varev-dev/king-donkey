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

	Level();
	~Level();
	void setUpLevel(int level);
	bool checkIsPlayerOnFinish();
	void updateBarrels(double currentTime);
	bool validateSetup() const;
	void resetLevel();
	void setCurrentColliders(Movable* movable);

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

	void addFloor(int x, int y, int width, int type = SAND) {
		if (floors_ctr == MAX_ELEMENTS || width > SCREEN_WIDTH)
			return;

		SDL_Surface* srf = nullptr;

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
};