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

#define TOP 2
#define BOTTOM 3

class Movable : public GameObject {
using GameObject::GameObject;
public:
	void move(int direction, double distance, GameObject block) {
		switch (direction) {
			case SDLK_LEFT:
				if (block.getBeginningAxisX() > (int)(this->getBeginningAxisX() - distance))
					this->setPositionX(block.getBeginningAxisX());
				else
					this->setPositionX(this->getBeginningAxisX() - distance);
				break;
			case SDLK_RIGHT:
				if (block.getEndAxisX() < this->getEndAxisX() + distance)
					this->setPositionX(block.getEndAxisX() - this->width);
				else
					this->setPositionX(this->getBeginningAxisX() + distance);
				break;
			case SDLK_UP:
				if (this->getBeginningAxisY() - distance > block.getBeginningAxisY() - this->height)
					this->setPositionY(this->getBeginningAxisY() - distance);
				else
					this->setPositionY(block.getBeginningAxisY() - this->height);
				break;
			case SDLK_DOWN:
				if (this->getEndAxisY() + distance < block.getEndAxisY())
					this->setPositionY(this->getBeginningAxisY() + distance);
				else
					this->setPositionY(block.getEndAxisY() - this->height);
				break;
		}
	}
};