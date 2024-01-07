#include "Movable.hpp"
#include "DEFINED_VALUES.h"

class Collider {
public:
	static bool CollisionBetweenMovableAndObject(GameObject go, Movable mv) {
		if (!CollisionOnAxisY(go, mv))
			return false;

		if (!CollisionOnAxisX(go, mv))
			return false;

		return true;
	}

	static bool IsMovePossibleOnAxisY(GameObject go, Movable mv) {
		if (go.getBeginningAxisX() > mv.getCenterOfAxisX())
			return false;

		if (go.getEndAxisX() < mv.getCenterOfAxisX())
			return false;

		if (!CollisionOnAxisY(go, mv))
			return false;

		return true;
	}

	static bool IsMovePossibleOnAxisX(GameObject go, Movable mv) {
		if (go.getBeginningAxisY() != mv.getEndAxisY())
			return false;

		if (!CollisionOnAxisX(go, mv))
			return false;

		return true;
	}

	static GameObject* SetCurrentCollider(GameObject *object[MAX_FLOORS], Movable* mv, int mode) {
		for (int i = 0; i < MAX_FLOORS; i++) {
			if (object[i] == nullptr)
				return nullptr;

			if (mode == FLOOR && IsMovePossibleOnAxisX(*object[i], *mv)) {
				return object[i];
			}

			if (mode == LADDER && IsMovePossibleOnAxisY(*object[i], *mv)) {
				return object[i];
			}
		}
	}

private:
	static bool CollisionOnAxisX(GameObject go, Movable mv) {
		if (go.getBeginningAxisX() > mv.getBeginningAxisX())
			return false;

		if (go.getEndAxisX() < mv.getEndAxisX())
			return false;

		return true;
	}

	static bool CollisionOnAxisY(GameObject go, Movable mv) {
		if (go.getEndAxisY() < mv.getBeginningAxisY())
			return false;
		
		if (go.getBeginningAxisY() > mv.getEndAxisY())
			return false;

		return true;
	}
};