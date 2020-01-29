//Dustin Gehm

#pragma once
#ifndef _RANDOM_H
#define _RANDOM_H

#define USE_PSEUDO 1

#if !USE_PSEUDO
#include <random>
#endif

#include "Singleton.h"

#if !USE_PSEUDO
using std::random_device;
#endif

class RandomManager : public Singleton<RandomManager>{
public:
	RandomManager();
	~RandomManager();

	static int randPosNeg();
	static bool randBool();
	static int randInt(int min, int max);
	static float randFloat(float min, float max);
	static double randDouble(double min, double max);

private:
#if !USE_PSEUDO
	static random_device device;
#endif
};

#endif