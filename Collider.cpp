#include "Collider.hpp"

bool Collider::CollisionBetweenMovableAndObject(GameObject go, Movable mv) {
	if (!CollisionOnAxisY(go, mv))
		return false;

	if (!CollisionOnAxisX(go, mv))
		return false;

	return true;
}

bool Collider::IsMovePossibleOnAxisY(GameObject go, Movable mv) {
	if (go.getBeginningAxisX() > mv.getCenterOfAxisX())
		return false;

	if (go.getEndAxisX() < mv.getCenterOfAxisX())
		return false;

	if (!CollisionOnAxisY(go, mv))
		return false;

	return true;
}

bool Collider::IsMovePossibleOnAxisX(GameObject go, Movable mv) {
	if (go.getBeginningAxisY() != mv.getEndAxisY())
		return false;

	if (!CollisionOnAxisX(go, mv))
		return false;

	return true;
}

GameObject* Collider::GetCollider(GameObject* object[MAX_ELEMENTS], Movable* mv, int mode) {
	for (int i = 0; i < MAX_ELEMENTS; i++) {
		if (object[i] == nullptr)
			return nullptr;

		if (mode == FLOOR && IsMovePossibleOnAxisX(*object[i], *mv)) {
			return object[i];
		}

		if (mode == LADDER && IsMovePossibleOnAxisY(*object[i], *mv)) {
			return object[i];
		}
	}

	return nullptr;
}

GameObject* Collider::GetNearestFloor(GameObject* object[MAX_ELEMENTS], Movable* mv) {
	GameObject* nearest = nullptr;

	for (int i = MAX_ELEMENTS - 1; i >= 0; i--) {
		if (object[i] == nullptr)
			continue;

		if (!CollisionOnAxisX(*object[i], *mv))
			continue;

		if (object[i]->getBeginningAxisY() < mv->getEndAxisY())
			continue;

		return object[i];
	}
}

bool Collider::IsFloorExtended(GameObject* floors[MAX_ELEMENTS], GameObject* current, int mode) {
	for (int i = 0; i < MAX_ELEMENTS; i++) {
		if (floors[i] == current || floors[i] == nullptr)
			continue;

		if (floors[i]->getBeginningAxisY() != current->getBeginningAxisY())
			continue;

		if (floors[i]->getBeginningAxisX() >= GAME_END_X)
			continue;

		if (floors[i]->getEndAxisX() <= GAME_BEG_X)
			continue;

		if (FloorExtension(current, floors[i], mode))
			return true;
	}

	return false;
}

bool Collider::CollisionBetweenMovables(Movable* first, Movable* second) {
	if (first->getBeginningAxisY() > second->getEndAxisY())
		return false;

	if (first->getEndAxisY() < second->getBeginningAxisY())
		return false;

	if (first->getBeginningAxisX() > second->getCenterOfAxisX())
		return false;

	if (first->getEndAxisX() < second->getCenterOfAxisX())
		return false;

	return true;
}