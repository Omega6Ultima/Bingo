//Dustin Gehm

#pragma once
#ifndef _POSITIONAL_H
#define _POSITIONAL_H

#include "VecN.h"

class Positional{
public:
	Positional();
	Positional(int x, int y);
	Positional(VecN<int, 2> p);
	~Positional();

	void setPos(VecN<int, 2> p);
	inline VecN<int, 2> getPos() const{
		return static_cast<VecN<int, 2>>(pos);
	}

	void setPosX(int x);
	inline int getPosX() const{
		return static_cast<int>(pos[0]);
	}

	void setPosY(int y);
	inline int getPosY() const{
		return static_cast<int>(pos[1]);
	}

protected:
	VecN<float, 2> pos;
};

#endif