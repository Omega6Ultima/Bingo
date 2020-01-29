//Dustin Gehm

#pragma once
#ifndef _CONTROLSURFACE_H
#define _CONTROLSURFACE_H

#include "PhysicalObject.h"
#include "Surface.h"
#include "Timer.h"
#include "VecN.h"

class MovableSurface : public Surface, public PhysicalObject{
public:
	enum MoveDir{ MOV_UP, MOV_DOWN, MOV_LEFT, MOV_RIGHT };

	MovableSurface(string path, int posX, int posY, float fX, float fY);
	MovableSurface(string path, Color colorKey, int x, int y, float fX, float fY);
	~MovableSurface();

	void move(MoveDir dir);
	void update();

private:
	Timer timer;
	VecN<float, 2> force;
};

#endif