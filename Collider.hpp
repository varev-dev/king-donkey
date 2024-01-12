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

	static GameObject* GetCollider(GameObject *object[MAX_ELEMENTS], Movable* mv, int mode) {
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

	static GameObject* GetNearestFloor(GameObject* object[MAX_ELEMENTS], Movable *mv) {
		GameObject* nearest = nullptr;

		for (int i = MAX_ELEMENTS - 1; i >= 0; i--) {
			if (object[i] == nullptr)
				continue;
			
			if (!CollisionOnAxisX(*object[i], *mv))
				continue;

			if (object[i]->getBeginningAxisY() < mv->getEndAxisY())
				continue;

			/*if (nearest != nullptr &&
				object[i]->getBeginningAxisY() > nearest->getBeginningAxisY())
				continue;*/

			return object[i];
		}
	}

private:
	static bool CollisionOnAxisX(GameObject go, Movable mv) {
		if (go.getBeginningAxisX() > mv.getEndAxisX())
			return false;

		if (go.getEndAxisX() < mv.getBeginningAxisX())
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