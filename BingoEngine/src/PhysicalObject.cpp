//Dustin Gehm

#include "PhysicalObject.h"

#include "WindowManager.h"

using Bingo::Math::PhysicalObject;
using Bingo::Math::VecN;

PhysicalObject::PhysicalObject()
	: Positional({ 0, 0 }) {
	//
}

PhysicalObject::PhysicalObject(VecN<int, 2> p)
	: Positional(p) {
}

PhysicalObject::PhysicalObject(VecN<int, 2> p, VecN<float, 2> v)
	: Positional(p) {
	vel = v;
}

PhysicalObject::PhysicalObject(VecN<int, 2> p, VecN<float, 2> v, VecN<float, 2> a)
	: Positional(p) {
	vel = v;
	acc = a;
}

PhysicalObject::~PhysicalObject() {
	//
}

void PhysicalObject::setVel(VecN<float, 2> v) {
	vel = v;
}

void PhysicalObject::setVelX(float x) {
	vel[0] = x;
}

void PhysicalObject::setVelY(float y) {
	vel[1] = y;
}

void PhysicalObject::setAcc(VecN<float, 2> a) {
	acc = a;
}

void PhysicalObject::setAccX(float x) {
	acc[0] = x;
}

void PhysicalObject::setAccY(float y) {
	acc[1] = y;
}

void PhysicalObject::addForce(VecN<float, 2> f) {
	acc += f;
}

void PhysicalObject::addForceX(float x) {
	acc[0] += x;
}

void PhysicalObject::addForceY(float y) {
	acc[1] += y;
}

void PhysicalObject::setFriction(float fric) {
	frictionCoeff = 1.0f - fric;
}

void PhysicalObject::setGravityEffects(bool grav) {
	gravityEffects = grav;
}

void PhysicalObject::setGravityStr(float grav) {
	gravityStr = grav;
}

void PhysicalObject::setScreenBounding(bool bound, int rad, VecN<int, 4> bounds) {
	screenBounding = bound;
	radius = rad;
	screenBounds = bounds;
	objectBounds = VecN<int, 4>({ bounds[0] + radius, bounds[1] + radius, bounds[2] - radius, bounds[3] - radius });
}

void PhysicalObject::update(float dtime) {
	//add the velocity per second to position
	pos += vel * dtime;

	//apply friction to the velocity
	vel *= frictionCoeff;

	//add the acceleration to the velocity
	vel += acc;

	//apply friction to the acceleration
	// TODO add option for this or something
	//acc *= frictionCoeff;
	acc *= 0.9375;

	//If gravity effects are on, add it to the y acceleration
	if (gravityEffects) {
		acc[1] += gravityStr;
	}

	if (screenBounding) {
		if (static_cast<int>(pos[0]) < objectBounds[0]) {
			pos[0] = static_cast<float>(objectBounds[0]);
		}
		else if (static_cast<int>(pos[0]) > objectBounds[2]) {
			pos[0] = static_cast<float>(objectBounds[2]);
		}

		if (static_cast<int>(pos[1]) < objectBounds[1]) {
			pos[1] = static_cast<float>(objectBounds[1]);
		}
		else if (static_cast<int>(pos[1]) > objectBounds[3]) {
			pos[1] = static_cast<float>(objectBounds[3]);
		}
	}
}