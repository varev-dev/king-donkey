#include <stdio.h>

#include "GameObject.hpp"
#include "DEFINED_VALUES.h"
#include <math.h>

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
		direction[X_AXIS] = UNDEF;
		direction[Y_AXIS] = UNDEF;
		state = DEFAULT_STATE;
		floor = nullptr;
		ladder = nullptr;
		jumpPosition[X_AXIS] = GAME_BEG_X;
		jumpPosition[Y_AXIS] = GAME_BEG_Y;
		jumpDirection = UNDEF;
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

		double newX = this->getBeginningAxisX(), newY = 0;

		if (jumpDirection != UNDEF) {
			newX += distance * (jumpDirection == LEFT ? -1 : 1);
			double argument = x1 * M_PI / JUMP_WIDTH;

			if (argument <= M_PI)
				newY = startPosition[Y_AXIS] - JUMP_HEIGHT * sin(argument);
			else if (argument > M_PI && argument < M_PI + 1)
				newY = startPosition[Y_AXIS] - JUMP_HEIGHT * (acos(argument - M_PI) - M_PI / 2);
			else
				newY = getBeginningAxisY() + 3 * distance;
		} else {
			newY = this->getBeginningAxisY() - distance * sin(M_PI/3);

			if (newY <= startPosition[Y_AXIS] - JUMP_HEIGHT) {
				newY = startPosition[Y_AXIS] - JUMP_HEIGHT;
				setState(FALL_STATE);
			}
		}

		if (newY > floor->getBeginningAxisY() - height) {
			newY = floor->getBeginningAxisY() - height;
			setState(DEFAULT_STATE);
		}

		if (!this->setPositionX(newX))
			setState(FALL_STATE);
		if (!this->setPositionY(newY))
			setState(FALL_STATE);
	}

	void fall(double distance, GameObject *floor) {
		int direction = this->getDirection(X_AXIS);
		if (direction != UNDEF)
			setPositionX(getBeginningAxisX() - (direction == LEFT ? 1 : -1) * distance / 3);

		if (this->getEndAxisY() + distance > floor->getBeginningAxisY()) {
			this->setPositionY(floor->getBeginningAxisY() - this->getHeight());
			this->setState(DEFAULT_STATE);
		} else {
			this->setPositionY(this->getBeginningAxisY() + distance * sin(M_PI / 2));
		}
	}

	void setDirection(int direction, int type) {
		switch (direction) {
		case KEY_LEFT:
		case LEFT:
			this->direction[X_AXIS] = type == PRESSED ? LEFT : UNDEF;
			break;
		case KEY_RIGHT:
		case RIGHT:
			this->direction[X_AXIS] = type == PRESSED ? RIGHT : UNDEF;
			break;
		case KEY_UP:
		case UP:
			this->direction[Y_AXIS] = type == PRESSED ? UP : UNDEF;
			break;
		case KEY_DOWN:
		case DOWN:
			this->direction[Y_AXIS] = type == PRESSED ? DOWN : UNDEF;
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
			return UNDEF;

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