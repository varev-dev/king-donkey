#include "Movable.hpp"
#include "DEFINED_VALUES.h"

class Collider {
public:
	static bool CollisionBetweenMovableAndObject(GameObject go, Movable mv);
	static bool IsMovePossibleOnAxisY(GameObject go, Movable mv);
	static bool IsMovePossibleOnAxisX(GameObject go, Movable mv);
	static GameObject* GetCollider(GameObject* object[MAX_ELEMENTS], Movable* mv, int mode);
	static GameObject* GetNearestFloor(GameObject* object[MAX_ELEMENTS], Movable* mv);
	static bool IsFloorExtended(GameObject* floors[MAX_ELEMENTS], GameObject* current, int mode);
	static bool CollisionBetweenMovables(Movable* first, Movable* second);

private:
	static bool FloorExtension(GameObject* current, GameObject* check, int mode) {
		if (mode != END_MODE && mode != BEG_MODE)
			return false;

		if (mode == BEG_MODE && current->getBeginningAxisX() != check->getEndAxisX())
			return false;

		if (mode == END_MODE && current->getEndAxisX() != check->getBeginningAxisX())
			return false;

		return true;
	}

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