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
	GameObject(SDL_Surface* sprite, double x, double y) {
		this->sprite = sprite;
		setPosition(x,y);
		this->width = sprite->w;
		this->height = sprite->h;
	}

	GameObject(SDL_Surface* sprite, double x, double y, int width, int height) {
		this->sprite = sprite;
		setPosition(x, y);
		this->width = width;
		this->height = height;
	}

	~GameObject() {
		//sprite = nullptr;
	}

	// setters
	void setSize(int width, int height) {
		this->width = width;
		this->height = height;
	}

	void setPosition(double x, double y, int limit = 0) {
		setPositionX(x, limit);
		setPositionY(y, limit);
	}

	bool setPositionX(double x, int limit = 0) {
		if (limit && (x < GAME_BEG_X || x + this->width >= GAME_END_X)) {
			printf("ERROR: Trying to set position X off screen.\n");
			return false;
		}

		this->x = x;
		return true;
	}

	bool setPositionY(double y, int limit = 0) {
		if (limit && (y < 0 || y + this->height >= GAME_END_Y)) {
			printf("ERROR: Trying to set position Y off screen.\n");
			return false;
		}

		this->y = y;
		return true;
	}

	void setSprite(SDL_Surface* sprite) {
		if (sprite == nullptr) {
			this->sprite = nullptr;
			return;
		}

		if (sprite->w > this->width || sprite->h > this->height) {
			printf("ERROR: The sprite is larger than the object itself.");
			return;
		}

		if (this->width % sprite->w != 0 || this->height % sprite->h != 0) {
			printf("ERROR: The sprite does not fit in object.");
			return;
		}

		this->sprite = sprite;
	}

	// getters
	int getHeight() {
		return this->height;
	}

	int getWidth() {
		return this->width;
	}

	double getBeginningAxisX() {
		return this->x;
	}

	double getEndAxisX() {
		return this->x + this->width;
	}

	double getBeginningAxisY() {
    	return this->y;
	}

	double getEndAxisY() {
		return this->y + this->height;
	}

	double getCenterOfAxisX() {
		return (this->x + this->x + width) / 2;
	}

	double getCenterOfAxisY() {
		return (this->y + this->y + height) / 2;
	}

	// prints
	void printObject() {
		printf("w: %d h: %d, ", width, height);
		printf("pos: (%f, %f), ", x, y);
		printf("sprite - w: %d h: %d\n", sprite->w, sprite->h);
	}

	void printOnScreen(SDL_Surface *screen) {
		SDL_Rect destination;

		for (int y = 0; y < this->height; y += this->sprite->h) {
			for (int x = 0; x < this->width; x += this->sprite->w) {
				destination.x = this->x + x;
				destination.y = this->y + y;
				destination.w = this->sprite->w;
				destination.h = this->sprite->h;
				SDL_BlitSurface(this->sprite, NULL, screen, &destination);
			}
		}
	}
};