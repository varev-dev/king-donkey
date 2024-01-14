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
	Movable(SDL_Surface* sprite, double x, double y);
	void setFloor(GameObject* floor);
	void setLadder(GameObject* ladder);
	GameObject* getFloor();
	GameObject* getLadder();
	void move(int direction, double distance, int limit = 1);
	void jump(double distance, double startPosition[2], int limit = 1);
	void fall(double distance, GameObject* floor, int limit = 1);
	void setDirection(int direction, int type);
	void switchDirection(int axis);
	void setState(int state);
	int getDirection(int axis);
	int getState();
	double* getJumpPosition();
	void setJumpDirection(int direction);

private:
	int direction[2];
	double jumpPosition[2];
	int jumpDirection;
	int state;
	GameObject *floor, *ladder;
};