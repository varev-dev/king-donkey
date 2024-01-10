#include <stdio.h>

#include "GameObject.hpp"
#include "DEFINED_VALUES.h"
#include <math.h>

extern "C" {
	#include"./SDL2-2.0.10/include/SDL.h"
	#include"./SDL2-2.0.10/include/SDL_main.h"
}

#define JUMP_WIDTH 96
#define JUMP_HEIGHT 48

#define PRESSED 1
#define RELEASED 0

#define DEFAULT_STATE 0
#define JUMP_STATE 1
#define FALL_STATE 2

class Movable : public GameObject {
	using GameObject::GameObject;

public:
	Movable(SDL_Surface* sprite, double x, double y) {
		this->sprite = sprite;
		setPosition(x, y);
		this->width = sprite->w;
		this->height = sprite->h;

		// default state
		direction[X_AXIS] = -1;
		direction[Y_AXIS] = -1;
		state = DEFAULT_STATE;
		floor = nullptr;
		ladder = nullptr;
		jumpPosition[X_AXIS] = GAME_BEG_X;
		jumpPosition[Y_AXIS] = GAME_BEG_Y;
	}

	void setFloor(GameObject *floor) {
		this->floor = floor;
	}

	void setLadder(GameObject *ladder) {
		this->ladder = ladder;
	}

	GameObject *getFloor() {
		return this->floor;
	}

	GameObject *getLadder() {
		return this->ladder;
	}

	void move(int direction, double distance) {
		switch (direction) {
			case LEFT:
				if (floor->getBeginningAxisX() > (int)(this->getEndAxisX() - distance))
					setState(FALL_STATE);

				this->setPositionX(this->getBeginningAxisX() - distance);
				break;
			case RIGHT:
				if (floor->getEndAxisX() < this->getBeginningAxisX() + distance)
					setState(FALL_STATE);

				this->setPositionX(this->getBeginningAxisX() + distance);
				break;
			case UP:
				if (this->getBeginningAxisY() - distance > ladder->getBeginningAxisY() - this->height) {
					this->setPositionY(this->getBeginningAxisY() - distance);
					this->setPositionX(ladder->getBeginningAxisX() - (this->getWidth() - ladder->getWidth()) / 2);
				} else
					this->setPositionY(ladder->getBeginningAxisY() - this->height);
				break;
			case DOWN:
				if (this->getEndAxisY() + distance < ladder->getEndAxisY()) {
					this->setPositionX(ladder->getBeginningAxisX() - (this->getWidth() - ladder->getWidth()) / 2);
					this->setPositionY(this->getBeginningAxisY() + distance);
				} else
					this->setPositionY(ladder->getEndAxisY() - this->height);
				break;
		}
	}

	// TO-DO
	void jump(double distance, double startPosition[2]) {
		double y1 = this->getEndAxisY() - startPosition[Y_AXIS] + this->height,
			x1 = this->getBeginningAxisX() - startPosition[X_AXIS];

		if (x1 < 0)
			x1 *= -1;

		double newX = this->getBeginningAxisX(), newY;

		if (jumpDirection == LEFT)
			newX -= distance;
		if (jumpDirection == RIGHT)
			newX += distance;

		if (jumpDirection != -1) {
			double argument = x1 * M_PI / JUMP_WIDTH;
			newY = startPosition[Y_AXIS] - JUMP_HEIGHT * sin(argument);
			if (newY > startPosition[Y_AXIS] && this->getBeginningAxisX() != startPosition[X_AXIS]) {
				newY = startPosition[Y_AXIS];
				setState(FALL_STATE);
			}
		} else {
			newY = this->getBeginningAxisY() - distance;

			if (newY <= startPosition[Y_AXIS] - JUMP_HEIGHT) {
				newY = startPosition[Y_AXIS] - JUMP_HEIGHT;
				setState(FALL_STATE);
			}
		}
		
		if (!this->setPositionX(newX))
			setState(FALL_STATE);
		if (!this->setPositionY(newY))
			setState(FALL_STATE);
	}

	void fall(double distance, GameObject *floor) {
		if (this->getEndAxisY() + distance > floor->getBeginningAxisY()) {
			this->setPositionY(floor->getBeginningAxisY() - this->getHeight());
			this->setState(DEFAULT_STATE);
		} else {
			this->setPositionY(this->getBeginningAxisY() + distance);
		}
	}

	void setDirection(int direction, int type) {
		switch (direction) {
		case SDLK_LEFT:
			this->direction[X_AXIS] = type == PRESSED ? LEFT : -1;
			break;
		case SDLK_RIGHT:
			this->direction[X_AXIS] = type == PRESSED ? RIGHT : -1;
			break;
		case SDLK_UP:
			this->direction[Y_AXIS] = type == PRESSED ? UP : -1;
			break;
		case SDLK_DOWN:
			this->direction[Y_AXIS] = type == PRESSED ? DOWN : -1;
			break;
		}
	}

	void setState(int state) {
		this->state = state;

		if (state == JUMP_STATE) {
			jumpPosition[X_AXIS] = this->getBeginningAxisX();
			jumpPosition[Y_AXIS] = this->getBeginningAxisY();
		}
	}

	int getDirection(int axis) {
		if (axis != X_AXIS && axis != Y_AXIS)
			return -1;

		return direction[axis];
	}

	int getState() {
		return this->state;
	}

	double* getJumpPosition() {
		return jumpPosition;
	}

	void setJumpDirection(int direction) {
		this->jumpDirection = direction;
	}

private:
	int direction[2];
	double jumpPosition[2];
	int jumpDirection;
	int state;
	GameObject *floor, *ladder;
};