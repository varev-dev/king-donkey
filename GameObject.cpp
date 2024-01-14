#include "GameObject.hpp"

// constr/destr
GameObject::GameObject(SDL_Surface* sprite, double x, double y) {
	this->sprite = sprite;
	setPosition(x, y);
	this->width = sprite->w;
	this->height = sprite->h;
}

GameObject::GameObject(SDL_Surface* sprite, double x, double y, int width, int height) {
	this->sprite = sprite;
	setPosition(x, y);
	this->width = width;
	this->height = height;
}

GameObject::~GameObject() {
	sprite = nullptr;
}

// setters
void GameObject::setSize(int width, int height) {
	this->width = width;
	this->height = height;
}

void GameObject::setPosition(double x, double y, int limit) {
	setPositionX(x, limit);
	setPositionY(y, limit);
}

bool GameObject::setPositionX(double x, int limit) {
	if (limit && (x < GAME_BEG_X || x + this->width >= GAME_END_X)) {
		printf("ERROR: Trying to set position X off screen.\n");
		return false;
	}

	this->x = x;
	return true;
}

bool GameObject::setPositionY(double y, int limit) {
	if (limit && (y < 0 || y + this->height >= GAME_END_Y)) {
		printf("ERROR: Trying to set position Y off screen.\n");
		return false;
	}

	this->y = y;
	return true;
}

void GameObject::setSprite(SDL_Surface* sprite) {
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
int GameObject::getHeight() {
	return this->height;
}

int GameObject::getWidth() {
	return this->width;
}

double GameObject::getBeginningAxisX() {
	return this->x;
}

double GameObject::getEndAxisX() {
	return this->x + this->width;
}

double GameObject::getBeginningAxisY() {
	return this->y;
}

double GameObject::getEndAxisY() {
	return this->y + this->height;
}

double GameObject::getCenterOfAxisX() {
	return (this->x + this->x + width) / 2;
}

double GameObject::getCenterOfAxisY() {
	return (this->y + this->y + height) / 2;
}

// prints
void GameObject::printObject() {
	printf("w: %d h: %d, ", width, height);
	printf("pos: (%f, %f), ", x, y);
	printf("sprite - w: %d h: %d\n", sprite->w, sprite->h);
}

void GameObject::printOnScreen(SDL_Surface* screen) {
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