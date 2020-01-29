//Dustin Gehm

#include "MovableSurface.h"

#include "WindowManager.h"

MovableSurface::MovableSurface(string path, int posX, int posY, float fX, float fY)
: Surface(path),
  PhysicalObject(VecN<int, 2>({ posX, posY })){
	force = { fX, fY };
}

MovableSurface::MovableSurface(string path, Color colorKey, int posX, int posY, float fX, float fY)
: Surface(path, colorKey),
  PhysicalObject(VecN<int, 2>({ posX, posY })){
	force = { fX, fY };
}

MovableSurface::~MovableSurface(){
	//
}

void MovableSurface::move(MoveDir dir){
	switch (dir){
	case  MOV_UP:
		addForceY(-force[1]);
		break;
	case  MOV_DOWN:
		addForceY(force[1]);
		break;
	case  MOV_LEFT:
		addForceX(-force[0]);
		break;
	case  MOV_RIGHT:
		addForceX(force[0]);
		break;
	}
}

void MovableSurface::update(){
	timer.end();
	timer.start();

	float dtime = Timer::ms_to_sec(timer.diff());

	VecN<int, 2> centerDiff = getCenter() - getPos();

	PhysicalObject::update(dtime);

	setCenter(getPos() + centerDiff);
}