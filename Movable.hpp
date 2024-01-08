#include <stdio.h>

#include "GameObject.hpp"
#include "DEFINED_VALUES.h"

extern "C" {
	#include"./SDL2-2.0.10/include/SDL.h"
	#include"./SDL2-2.0.10/include/SDL_main.h"
}

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

class Movable : public GameObject {
using GameObject::GameObject;
public:
	void setFloor(GameObject *floor) {
		this->floor = floor;
	}

	void setLadder(GameObject *ladder) {
		this->ladder = ladder;
	}

	GameObject *getFloor() {
		return this->floor;
	}

	GameObject* getLadder() {
		return this->ladder;
	}

	void move(int direction, double distance) {
		switch (direction) {
			case SDLK_LEFT:
			case LEFT:
				if (floor->getBeginningAxisX() > (int)(this->getBeginningAxisX() - distance))
					this->setPositionX(floor->getBeginningAxisX());
				else
					this->setPositionX(this->getBeginningAxisX() - distance);
				break;
			case SDLK_RIGHT:
			case RIGHT:
				if (floor->getEndAxisX() < this->getEndAxisX() + distance)
					this->setPositionX(floor->getEndAxisX() - this->width);
				else
					this->setPositionX(this->getBeginningAxisX() + distance);
				break;
			case SDLK_UP:
			case UP:
				if (this->getBeginningAxisY() - distance > ladder->getBeginningAxisY() - this->height)
					this->setPositionY(this->getBeginningAxisY() - distance);
				else
					this->setPositionY(ladder->getBeginningAxisY() - this->height);
				break;
			case SDLK_DOWN:
			case DOWN:
				if (this->getEndAxisY() + distance < ladder->getEndAxisY())
					this->setPositionY(this->getBeginningAxisY() + distance);
				else
					this->setPositionY(ladder->getEndAxisY() - this->height);
				break;
		}
	}
private:
	GameObject *floor, *ladder;
};