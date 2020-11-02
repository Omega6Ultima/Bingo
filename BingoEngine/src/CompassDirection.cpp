//Dustin Gehm

#include "CompassDirection.h"

#include "Exception.h"

using Bingo::Direction::CompassDirection;
using Bingo::Math::VecN;

CompassDirection::CompassDirection() {
	vertical = 0;
	horizontal = 0;
}

CompassDirection::CompassDirection(char dir) {
#if _DEBUG
	if (!(dir == 'N' || dir == 'E' || dir == 'S' || dir == 'W')) {
		throw Exception("CompassDirection not set to a compass direction: " + dir);
	}
#endif
	switch (dir) {
	case 'N':
	case 'S':
		vertical = dir;
		horizontal = 0;
		break;
	case 'E':
	case 'W':
		vertical = 0;
		horizontal = dir;
		break;
	}
}

CompassDirection::CompassDirection(char dir, char dir2) {
#if _DEBUG
	if (!(dir == 'N' || dir == 'S')) {
		throw Exception("CompassDirection not set to a compass direction: " + dir);
	}
	if (!(dir2 == 'E' || dir2 == 'W')) {
		throw Exception("CompassDirection not set to a compass direction: " + dir2);
	}
#endif

	vertical = dir;
	horizontal = dir2;
}

CompassDirection::CompassDirection(VecN<float, 2> vec) {
	if (vec[0] == 0) {
		horizontal = 0;
	}
	else if (vec[0] > 0) {
		horizontal = 'E';
	}
	else {
		horizontal = 'W';
	}

	if (vec[1] == 0) {
		vertical = 0;
	}
	else if (vec[1] > 0) {
		vertical = 'S';
	}
	else {
		vertical = 'N';
	}
}

CompassDirection::~CompassDirection() {
	//
}

VecN<int, 2> CompassDirection::getMultipliers(int defaultVal) const {
	VecN<int, 2> result({ defaultVal, defaultVal });

	if (vertical != 0) {
		result[1] = 1;
	}

	if (horizontal != 0) {
		result[0] = 1;
	}

	return result;
}

CompassDirection CompassDirection::operator+(const CompassDirection& cd) const {
	CompassDirection result(*this);

	if (result.vertical == 0) {
		result.vertical = cd.vertical;
	}
	else if (result.vertical != cd.vertical) {
		result.vertical = 0;
	}

	if (result.horizontal == 0) {
		result.horizontal = cd.horizontal;
	}
	else if (result.horizontal != cd.horizontal) {
		result.horizontal = 0;
	}

	return result;
}

CompassDirection CompassDirection::operator-(const CompassDirection& cd) const {
	CompassDirection result(*this);

	if (result.vertical == cd.vertical) {
		result.vertical = 0;
	}

	if (result.horizontal == cd.horizontal) {
		result.horizontal = 0;
	}

	return result;
}

void CompassDirection::operator+=(const CompassDirection& cd) {
	if (vertical == 0) {
		vertical = cd.vertical;
	}
	else if (vertical != cd.vertical) {
		vertical = 0;
	}

	if (horizontal == 0) {
		horizontal = cd.horizontal;
	}
	else if (horizontal != cd.horizontal) {
		horizontal = 0;
	}
}

void CompassDirection::operator-=(const CompassDirection& cd) {
	if (vertical == cd.vertical) {
		vertical = 0;
	}

	if (horizontal == cd.horizontal) {
		horizontal = 0;
	}
}