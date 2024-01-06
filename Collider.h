#include "Movable.h"

class Collider {
public:
	static bool CollisionBetweenMovableAndObject(GameObject go, Movable mv) {
		if (go.getEndAxisY() < mv.getBeginningAxisY())
			return false;

		if (go.getBeginningAxisY() > mv.getEndAxisY())
			return false;

		if (go.getBeginningAxisX() > mv.getCenterOfAxisX())
			return false;

		if (go.getEndAxisX() < mv.getCenterOfAxisX())
			return false;

		return true;
	}

	static bool IsMovePossibleOnAxisY(GameObject go, Movable mv) {
		if (go.getBeginningAxisX() > mv.getCenterOfAxisX()) {
			printf("1\n");
			return false;
		}

		if (go.getEndAxisX() < mv.getCenterOfAxisX()) {
			printf("1\n");
			return false;
		}

		if (!CollisionOnAxisY(go, mv)) {
			return false;
		}

		return true;
	}

	static bool IsMovePossibleOnAxisX(GameObject go, Movable mv) {
		if (go.getBeginningAxisY() != mv.getEndAxisY()) {
			printf("1\n");
			return false;
		}

		if (!CollisionOnAxisX(go, mv)) {
			printf("2\n");
			return false;
		}

		return true;
	}

private:
	static bool CollisionOnAxisX(GameObject go, Movable mv) {
		if (go.getBeginningAxisX() > mv.getCenterOfAxisX())
			return false;

		if (go.getEndAxisX() < mv.getCenterOfAxisX())
			return false;

		return true;
	}

	static bool CollisionOnAxisY(GameObject go, Movable mv) {
		if (go.getEndAxisY() < mv.getBeginningAxisY()) {
			printf("3\n");
			return false;
		}
		if (go.getBeginningAxisY() > mv.getEndAxisY()) {
			printf("4\n");
			return false;
		}
		return true;
	}
};