#include <stdio.h>
#include "DEFINED_VALUES.h"

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

class GameObject {
protected:
	SDL_Surface* sprite; // texture
	double x, y; // position
	int width, height; // size
	GameObject() = default;

public:
	// contructors
	GameObject(SDL_Surface* sprite, double x, double y);
	GameObject(SDL_Surface* sprite, double x, double y, int width, int height);
	~GameObject();

	// setters
	void setSize(int width, int height);
	void setPosition(double x, double y, int limit = 0);
	bool setPositionX(double x, int limit = 0);
	bool setPositionY(double y, int limit = 0);
	void setSprite(SDL_Surface* sprite);

	// getters
	int getHeight();
	int getWidth();
	double getBeginningAxisX();
	double getEndAxisX();
	double getBeginningAxisY();
	double getEndAxisY();
	double getCenterOfAxisX();
	double getCenterOfAxisY();

	// prints
	void printObject();
	void printOnScreen(SDL_Surface* screen);
};