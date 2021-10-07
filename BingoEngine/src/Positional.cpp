//Dustin Gehm

#include "Positional.h"

using Bingo::Math::Positional;
using Bingo::Math::Positional3D;
using Bingo::Math::VecN;

Positional::Positional() {
	pos = { 0.0f, 0.0f };
}

Positional::Positional(int x, int y) {
	pos = { static_cast<float>(x), static_cast<float>(y) };
}

Positional::Positional(VecN<int, 2> p) {
	pos = static_cast<VecN<float, 2>>(p);
}

Positional::~Positional() {
	//
}

void Positional::setPos(VecN<int, 2> p) {
	pos = p;
}

void Positional::setPosX(int x) {
	pos[0] = static_cast<float>(x);
}

void Positional::setPosY(int y) {
	pos[1] = static_cast<float>(y);
}

Positional3D::Positional3D() {
	pos = { 0.0, 0.0, 0.0 };
}

Positional3D::Positional3D(int x, int y, int z) {
	pos = { static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) };
}

Positional3D::Positional3D(VecN<int, 3> p) {
	pos = p;
}

Positional3D::~Positional3D() {
	//
}

void Positional3D::setPos(VecN<int, 3> p) {
	pos = p;
}

void Positional3D::setPosX(int x) {
	pos[0] = static_cast<float>(x);
}

void Positional3D::setPosY(int y) {
	pos[1] = static_cast<float>(y);
}

void Positional3D::setPosZ(int z) {
	pos[2] = static_cast<float>(z);
}
