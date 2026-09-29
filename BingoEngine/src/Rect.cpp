//

#include <memory>

#include "Rect.h"

using Bingo::Rect;

Rect::Rect(int x, int y, int w, int h) {
	values[0] = x;
	values[1] = y;
	values[2] = w;
	values[3] = h;
}

void Rect::setX(int x) {
	values[0] = x;
}

void Rect::setY(int y) {
	values[1] = y;
}

void Rect::setW(int w) {
	values[2] = w;
}

void Rect::setH(int h) {
	values[3] = h;
}

Rect::Rect(const Rect& other) {
	std::memcpy(&values, &other.values, sizeof(values));
}