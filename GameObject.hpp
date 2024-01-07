#include <stdio.h>
#include "DEFINED_VALUES.h"

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

class GameObject {
protected:
	SDL_Surface* sprite; // texture
	char* name; // name, for test purposes
	int x, y; // position
	int width, height; // size
public:
	// contructors
	GameObject() = default;

	GameObject(SDL_Surface* sprite, char *name, int x, int y, int width, int heigth) {
		this->sprite = sprite;
		this->name = name;
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = heigth;
	}

	// setters
	void setSize(int width, int height) {
		this->width = width;
		this->height = height;
	}

	void setPosition(int x, int y) {
		if (x < 0 || y < 0 || x + this->width >= SCREEN_WIDTH || y + this->height >= SCREEN_HEIGHT) {
			printf("ERROR: Trying to set position off screen.\n");
			return;
		}

		this->x = x;
		this->y = y;
	}

	void setPositionX(int x) {
		if (x < 0 || x + this->width >= SCREEN_WIDTH) {
			printf("ERROR: Trying to set position X off screen.\n");
			return;
		}

		this->x = x;
	}

	void setPositionY(int y) {
		if (y < 0 || y + this->height >= SCREEN_HEIGHT) {
			printf("ERROR: Trying to set position Y off screen.\n");
			return;
		}

		this->y = y;
	}

	void setSprite(SDL_Surface* sprite) {
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

	void setName(char* name) {
		this->name = name;
	}

	// getters
	int getHeight() {
		return this->height;
	}

	int getWidth() {
		return this->width;
	}

	int getBeginningAxisX() {
		return this->x;
	}

	int getEndAxisX() {
		return this->x + this->width;
	}

	int getBeginningAxisY() {
		return this->y;
	}

	int getEndAxisY() {
		return this->y + this->height;
	}

	int getCenterOfAxisX() {
		return (this->x + this->x + width) / 2;
	}

	int getCenterOfAxisY() {
		return (this->y + this->y + height) / 2;
	}

	// prints
	void printObject() {
		printf("\"%s\" ", name);
		printf("w: %d h: %d, ", width, height);
		printf("pos: (%d, %d), ", x, y);
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