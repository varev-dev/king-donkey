#include <stdio.h>

#include "GameObject.hpp"
#include "DEFINED_VALUES.h"
#include <math.h>

extern "C" {
	#include"./SDL2-2.0.10/include/SDL.h"
	#include"./SDL2-2.0.10/include/SDL_main.h"
}

#define JUMP_WIDTH 128
#define JUMP_HEIGHT 48

#define PRESSED 1
#define RELEASED 0

#define DEFAULT_STATE 0
#define JUMP_STATE 1
#define FALL_STATE 2

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

	GameObject *getLadder() {
		return this->ladder;
	}

	void move(int direction, double distance) {
		switch (direction) {
			case LEFT:
				if (floor->getBeginningAxisX() > (int)(this->getBeginningAxisX() - distance))
					this->setPositionX(floor->getBeginningAxisX());
				else
					this->setPositionX(this->getBeginningAxisX() - distance);
				break;
			case RIGHT:
				if (floor->getEndAxisX() < this->getEndAxisX() + distance)
					this->setPositionX(floor->getEndAxisX() - this->width);
				else
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
	void jump(int direction, double distance, double startPosition[2]) {
		double y1 = this->getEndAxisY() - startPosition[Y_AXIS] + this->height,
			x1 = this->getBeginningAxisX() - startPosition[X_AXIS];

		if (x1 < 0)
			x1 *= -1;

		printf("%f %f\t", startPosition[X_AXIS], startPosition[Y_AXIS]);

		double newX = this->getBeginningAxisX();

		if (direction == LEFT)
			newX -= distance;
		if (direction == RIGHT)
			newX += distance;

		double argument = x1 * M_PI / JUMP_WIDTH;
		double newY = startPosition[Y_AXIS] - JUMP_HEIGHT * sin(argument);

		if (newY >= startPosition[Y_AXIS] && this->getBeginningAxisX() != startPosition[X_AXIS]) {
			newY = startPosition[Y_AXIS];
			setState(DEFAULT_STATE);
		}
		
		printf("%f %f\n", newX, newY);
		this->printObject();
		//this->setPositionX(newX);
		this->setPosition(newX, newY);
	}

	void fall(double distance) {

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

private:
	int direction[2];
	double jumpPosition[2];
	int state;
	GameObject *floor, *ladder;
};