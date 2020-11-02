//Dustin Gehm

#include "BoundsManager.h"

#include <math.h>

#include "WindowManager.h"
#include "Utils.h"

using std::cos;
using std::sqrt;

using Bingo::BoundsManager;
using Bingo::Collision::BoundsBase;
using Bingo::Collision::BoundsBox;
using Bingo::Collision::BoundsCircle;
using Bingo::Collision::BoundsLine;
using Bingo::Colors::Color;
using Bingo::Direction::CompassDirection;
using Bingo::Math::VecN;
using Bingo::Surfaces::WindowManager;
using Bingo::Time::Timer;
using Bingo::TimeConvert::ms_to_sec;
using Bingo::Utils::degToRadians;

BoundsManager::BoundsManager() {
	int windowWidth = WindowManager::getSingleton().getWidth();
	int windowHeight = WindowManager::getSingleton().getHeight();

	screenEdges[0] = new BoundsLine({ 0, 0 }, { 0, windowHeight }, Colors::BLACK);
	screenEdges[1] = new BoundsLine({ 0, windowHeight }, { windowWidth, windowHeight }, Colors::BLACK);
	//screenEdges[2] = new BoundsLine({ windowWidth, windowHeight }, { windowWidth, 0 }, Colors::BLACK);
	screenEdges[2] = new BoundsLine({ windowWidth, 0 }, { windowWidth, windowHeight }, Colors::BLACK);
	//screenEdges[3] = new BoundsLine({ windowWidth, 0 }, { 0, 0 }, Colors::BLACK);
	screenEdges[3] = new BoundsLine({ 0, 0 }, { windowWidth, 0 }, Colors::BLACK);
}

BoundsManager::~BoundsManager() {
	for (uint c = 0; c < 4; c++) {
		delete screenEdges[c];
	}
}

void BoundsManager::addBounds(BoundsBase* b) {
	bounds.push_back(b);
}

void BoundsManager::update() {
	timer.end();
	timer.start();

	int windowWidth = WindowManager::getSingleton().getWidth();
	int windowHeight = WindowManager::getSingleton().getHeight();
	float dtime = ms_to_sec(timer.diff());

	//update positions and velocities and check screen collisions
	for (uint c = 0; c < bounds.size(); c++) {
		bounds[c]->update(dtime);

		if (bounds[c]->reflectScreenEdge) {
			//d is an int because we're going to use a theoretical negative index as a key
			for (int d = 0; d < 4; d++) {
				if (intersect(bounds[c]->getRoughArea(), screenEdges[d]->getRoughArea())) {
					VecN<int, 2> point2 = screenEdges[d]->getClosestPoint(bounds[c]->getPos());
					VecN<int, 2> point1 = bounds[c]->getClosestPoint(point2);

					if (distanceSq(point1, point2) < 2) {
						uint curTicks = timer.getTicks();
						pair<int, int> key(c, -(d + 1));

						if (cooldowns.find(key) == cooldowns.end() ||
							curTicks - cooldowns[key] >= cooldown) {

							CompassDirection direct(bounds[c]->getVel());

							handleCollision(bounds[c], screenEdges[d]->getNormal(point2, direct));
							cooldowns[key] = curTicks;
						}
					}
				}
			}
		}

		//check collisions between bounds
		for (uint d = c + 1; d < bounds.size(); d++) {
			if (intersect(bounds[c]->getRoughArea(), bounds[d]->getRoughArea())) {
				VecN<int, 2> point2 = bounds[d]->getClosestPoint(bounds[c]->getPos());
				VecN<int, 2> point1 = bounds[c]->getClosestPoint(point2);

				if (distanceSq(point1, point2) < 2) {
					uint curTicks = timer.getTicks();
					pair<int, int> key(c, d);

					if (cooldowns.find(key) == cooldowns.end() ||
						curTicks - cooldowns[key] >= cooldown) {

						CompassDirection direct1(bounds[c]->getVel());
						CompassDirection direct2(bounds[d]->getVel());

						handleCollision(bounds[c], bounds[d]->getNormal(point2, direct1));
						handleCollision(bounds[d], bounds[c]->getNormal(point1, direct2));
						cooldowns[key] = curTicks;
						cooldowns[pair<int, int>(d, c)] = curTicks;
					}
				}
			}
		}
	}
}

void BoundsManager::draw(Surfaces::Surface& surf) {
	for (uint c = 0; c < bounds.size(); c++) {
		bounds[c]->draw(surf, false);

#if DEBUG_DRAWS_FINE
		for (uint d = 0; d < bounds.size(); d++) {
			if (c == d) continue;
			VecN<int, 2> point2 = bounds[d]->getClosestPoint(bounds[c]->getPos());
			VecN<int, 2> point1 = bounds[c]->getClosestPoint(point2);

			Color oldColor = surf.getDrawColor();

			surf.setDrawColor(bounds[d]->color);

			//surf.drawPoint(point1);
			surf.drawCircle(point1, 2, false);

			surf.setDrawColor(oldColor);
		}
#endif
	}

#if DEBUG_DRAWS_ROUGH
	for (uint c = 0; c < 4; c++) {
		screenEdges[c]->draw(surf);
	}
#endif
}

void BoundsManager::handleCollision(BoundsBase* base, VecN<float, 2> normal) {
	//collision happened, reflect the velocities
	//cerr << "collision!\n";

	if (base->onCollide) {
		base->onCollide();
	}

	//maybe reflect around a point instead on just negating the vel
	//base->setVel(base->getVel() * (-1 + base->reflectStr));
	VecN<float, 2> vel = base->getVel();
	float dotProd = normal.dot(vel);
	VecN<float, 2> newVel = vel - (normal * 2 * dotProd);
	base->setVel(newVel);
}

BoundsBase::BoundsBase(VecN<int, 2> pos, Color col)
	: PhysicalObject(pos), color(col) {
	//
}

BoundsBase::~BoundsBase() {
	//
}

void BoundsBase::setReflectStr(float str) {
	reflectStr = str;
}

void BoundsBase::setReflectScreenEdge(bool reflect) {
	reflectScreenEdge = reflect;
}

BoundsCircle::BoundsCircle(VecN<int, 2> pos, uint radius, Color col)
	: BoundsBase(pos, col) {
	rad = radius;
}

BoundsCircle::~BoundsCircle() {
	//
}

void BoundsCircle::setRad(uint radius) {
	rad = radius;
}

VecN<int, 4> BoundsCircle::getRoughArea() const {
	VecN<int, 2> pos = getPos();
	int adjRad = rad + 5;

	return{ pos[0] - adjRad, pos[1] - adjRad, adjRad + adjRad, adjRad + adjRad };
}

VecN<int, 2> BoundsCircle::getClosestPoint(VecN<int, 2> p) {
	//VecN<int, 2> p;

	//if (b->getType() == BoundsManager::LINE){
	//	p = b->getClosestPoint(this);
	//}
	//else{
	//	p = b->getCenter();
	//}

	VecN<int, 2> pos = getPos();
	VecN<float, 2> dir = (p - pos);

	dir.normalize();

	return pos + (dir * static_cast<float>(rad));
}

VecN<float, 2> BoundsCircle::getNormal(VecN<int, 2> p, CompassDirection& dir) {
	return (p - getPos()).normalizeCopy();
}

BoundsManager::BoundsType BoundsCircle::getType() const {
	return BoundsManager::CIRCLE;
}

VecN<int, 2> BoundsCircle::getCenter() const {
	return getPos();
}

void BoundsCircle::draw(Surface& surf, bool filled) const {
	Color oldColor = surf.getDrawColor();

	surf.setDrawColor(color);

	surf.drawCircle(getPos(), rad, filled);

#if DEBUG_DRAWS_ROUGH
	VecN<int, 4> rect = getRoughArea();
	surf.drawRect(rect[0], rect[1], rect[2], rect[3], false);
#endif

	surf.setDrawColor(oldColor);
}

BoundsLine::BoundsLine(VecN<int, 2> pos, VecN<int, 2> pos2, Color col)
	: BoundsBase(pos, col) {
	this->pos2 = pos2;
}

BoundsLine::~BoundsLine() {
	//
}

VecN<int, 4> BoundsLine::getRoughArea() const {
	VecN<int, 2> pos = getPos();

	return{ pos[0] - 10, pos[1] - 10, MAX(pos2[0] - pos[0] + 20, 20), MAX(pos2[1] - pos[1] + 20, 20) };
}

VecN<int, 2> BoundsLine::getClosestPoint(VecN<int, 2> p) {
	//VecN<int, 2> p;
	VecN<int, 2> result;
	VecN<int, 2> pos = getPos();
	VecN<int, 2> closestPos;
	//I'm intentionally keeping this a double because of the return type from std::cos
	VecN<double, 2> dirLine;

	//if (b->getType() == BoundsManager::LINE){
	//	p = b->getCenter();
	//}
	//else{
	//	p = b->getCenter();
	//}

	if (distanceSq(pos, p) < distanceSq(pos2, p)) {
		closestPos = pos;
		dirLine = pos2 - pos;
	}
	else {
		closestPos = pos2;
		dirLine = pos - pos2;
	}

	double angRad = (p - closestPos).angleBetween(dirLine);

	//degToRadians(90) == 1.57081
	if (angRad >= degToRadians(90)) {
		result = closestPos;
	}
	else {
		result = closestPos + (VecN<int, 2>)((dirLine).normalizeCopy() * cos(angRad) * distance(closestPos, p));
	}

	return result;
}

VecN<float, 2> BoundsLine::getNormal(VecN<int, 2> p, CompassDirection& dir) {
	VecN<float, 2> dirLine(pos2 - p);
	VecN<float, 2> normal = dirLine.perpendicular();
	VecN<int, 2> mult = -dir.getMultipliers(1);

	if (normal[0] > 0 && mult[0] < 0) {
		normal[0] *= -1;
	}
	else if (normal[0] < 0 && mult[0] > 0) {
		normal[0] *= -1;
	}

	if (normal[1] > 0 && mult[1] < 0) {
		normal[1] *= -1;
	}
	else if (normal[1] < 0 && mult[1] > 0) {
		normal[1] *= -1;
	}

	return normal.normalizeCopy();
}

BoundsManager::BoundsType BoundsLine::getType() const {
	return BoundsManager::LINE;
}

VecN<int, 2> BoundsLine::getCenter() const {
	return (getPos() + pos2) / 2;
}

void BoundsLine::draw(Surface& surf, bool filled) const {
	Color oldColor = surf.getDrawColor();

	surf.setDrawColor(color);

	surf.drawLine(getPos(), pos2);

#if DEBUG_DRAWS_ROUGH
	VecN<int, 4> rect = getRoughArea();
	surf.drawRect(rect[0], rect[1], rect[2], rect[3], false);
#endif

	surf.setDrawColor(oldColor);
}

BoundsBox::BoundsBox(VecN<int, 2> pos, int w, int h, Color col)
	: BoundsBase(pos, col) {
	width = w;
	height = h;

	corners[0] = pos;
	corners[1] = pos.addTo(0, w);
	corners[2] = pos.addTo(1, h);
	corners[3] = pos.addTo(0, w).addTo(1, h);

	center = pos.addTo(0, w / 2).addTo(1, h / 2);
}

BoundsBox::~BoundsBox() {
	//
}

VecN<int, 4> BoundsBox::getRoughArea() const {
	return{ corners[0][0] - 10, corners[0][1] - 10, width + 20, height + 20 };
}

VecN<int, 2> BoundsBox::getClosestPoint(VecN<int, 2> p) {
	//subtract p and all 4 corners and find the 2 with the least distance
	//the shared x/y component of those is one component of the result
	//the other will be the x/y of p clamped between the two corners x/y
	//VecN<int, 2> p;

	//if (b->getType() == BoundsManager::LINE){
	//	p = b->getClosestPoint(this);
	//}
	//else{
	//	p = b->getCenter();
	//}

	VecN<int, 2> result;
	VecN<float, 4> lengths;
	uchar ind = 0, ind2 = 0;
	VecN<int, 2> lessCorner;
	VecN<int, 2> greatCorner;

	lengths[0] = distanceSq(p, corners[0]);
	lengths[1] = distanceSq(p, corners[1]);
	lengths[2] = distanceSq(p, corners[2]);
	lengths[3] = distanceSq(p, corners[3]);

	for (uint c = 1; c < 4; c++) {
		if (lengths[c] < lengths[ind]) {
			ind = c;
		}
	}

	for (uint c = 1; c < 4; c++) {
		if (c != ind && lengths[c] <= lengths[ind2]) {
			ind2 = c;
		}
	}

	if (corners[ind] < corners[ind2]) {
		lessCorner = corners[ind];
		greatCorner = corners[ind2];
	}
	else {
		lessCorner = corners[ind2];
		greatCorner = corners[ind];
	}

	if (lessCorner[0] == greatCorner[0]) {
		result[0] = lessCorner[0];

		result[1] = p[1];

		if (result[1] < lessCorner[1]) {
			result[1] = lessCorner[1];
		}
		else if (result[1] > greatCorner[1]) {
			result[1] = greatCorner[1];
		}
	}
	else {
		result[1] = lessCorner[1];

		result[0] = p[0];

		if (result[0] < lessCorner[0]) {
			result[0] = lessCorner[0];
		}
		else if (result[0] > greatCorner[0]) {
			result[0] = greatCorner[0];
		}
	}

	return result;
}

VecN<float, 2> BoundsBox::getNormal(VecN<int, 2> p, CompassDirection& dir) {
	VecN<float, 2> normal;

	for (int c = 0; c < 4; c++) {
		if (corners[c] == p) {
			return ((VecN<float, 2>)corners[c] - center).normalizeCopy();
		}
	}

	VecN<int, 2> mult = dir.getMultipliers(0);

	normal = (p - center) * mult;

	return normal.normalizeCopy();
}

BoundsManager::BoundsType BoundsBox::getType() const {
	return BoundsManager::BOX;
}

VecN<int, 2> BoundsBox::getCenter() const {
	return corners[0].addTo(0, width / 2).addTo(1, height / 2);
}

void BoundsBox::draw(Surface& surf, bool filled) const {
	Color oldColor = surf.getDrawColor();

	surf.setDrawColor(color);

	surf.drawRect(getPos(), width, height, filled);

#if DEBUG_DRAWS_ROUGH
	VecN<int, 4> rect = getRoughArea();
	surf.drawRect(rect[0], rect[1], rect[2], rect[3], false);
#endif

	surf.setDrawColor(oldColor);
}