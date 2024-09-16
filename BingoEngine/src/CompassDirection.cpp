//Dustin Gehm

#include "CompassDirection.h"

#include "Exception.h"

using Bingo::Direction::CompassDirection;
using Bingo::Math::VecN;

CompassDirection::CompassDirection() {
	//
}

CompassDirection::CompassDirection(char dir) {
	dir = tolower(dir);

#if _DEBUG
	if (!(dir == 'n' || dir == 'e' || dir == 's' || dir == 'w')) {
		throw Exception("CompassDirection not set to a compass direction: " + dir);
	}
#endif

	switch (dir) {
	case 'n':
		vertical = 1.0f;
		break;
	case 's':
		vertical = -1.0f;
		break;
	case 'e':
		horizontal = 1.0f;
		break;
	case 'w':
		horizontal = -1.0f;
		break;
	}
}

CompassDirection::CompassDirection(char dir, char dir2) {
	dir = tolower(dir);
	dir2 = tolower(dir2);

#if _DEBUG
	if (!(dir == 'n' || dir == 's')) {
		throw Exception("CompassDirection not set to a compass direction: " + dir);
	}
	if (!(dir2 == 'e' || dir2 == 'w')) {
		throw Exception("CompassDirection not set to a compass direction: " + dir2);
	}
#endif

	switch (dir) {
	case 'n':
		vertical = 1.0f;
		break;
	case 's':
		vertical = -1.0f;
		break;
	}

	switch (dir2) {
	case 'e':
		horizontal = 1.0f;
		break;
	case 'w':
		horizontal = -1.0f;
		break;
	}
}

CompassDirection::CompassDirection(VecN<float, 2> vec) {
	vertical = vec[1];
	horizontal = vec[0];
}

CompassDirection::~CompassDirection() {
	//
}

VecN<int, 2> CompassDirection::getMultipliers(int defaultVal) const {
	VecN<int, 2> result({ defaultVal, defaultVal });

	if (vertical != 0.0f) {
		result[1] = 1;
	}

	if (horizontal != 0.0f) {
		result[0] = 1;
	}

	return result;
}

CompassDirection CompassDirection::operator+(const CompassDirection& cd) const {
	CompassDirection result(*this);

	result.vertical += cd.vertical;
	result.horizontal += cd.horizontal;

	return result;
}

CompassDirection CompassDirection::operator-(const CompassDirection& cd) const {
	CompassDirection result(*this);

	result.vertical -= cd.vertical;
	result.horizontal -= cd.horizontal;

	return result;
}

void CompassDirection::operator+=(const CompassDirection& cd) {
	vertical += cd.vertical;
	horizontal += cd.horizontal;
}

void CompassDirection::operator-=(const CompassDirection& cd) {
	vertical -= cd.vertical;
	horizontal -= cd.horizontal;
}

bool CompassDirection::operator==(const CompassDirection& cd) const {
	return (vertical == cd.vertical && horizontal == cd.horizontal);
}