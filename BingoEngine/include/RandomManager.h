//Dustin Gehm

#pragma once
#ifndef _RANDOM_H
#define _RANDOM_H

constexpr auto USE_PSEUDO = 1;

#if !USE_PSEUDO
#include <random>
#endif

#include "Core.h"
#include "Singleton.h"

#if !USE_PSEUDO
using std::random_device;
#endif

namespace Bingo {

	using Core::Manager;

	class RandomManager : public Singleton<RandomManager>, public Manager {
	public:
		RandomManager();
		~RandomManager();

		static int randPosNeg();
		static bool randBool();
		static int randInt(int min, int max);
		static float randFloat(float min, float max);
		static double randDouble(double min, double max);
		static double perlinNoise2D(double x, double y, double depth = 5, double frequency = 0.7, int seed = 1234);
		static double perlinNoise3D(double x, double y, double z, double depth = 5, double frequency = 0.7, int seed = 1234);

	private:
		static double noise(double x, double y, int seed);
		static double noise(double x, double y, double z, int seed);
		static uchar hashFunc(int x, int y, int seed);
		static uchar hashFunc(int x, int y, int z, int seed);
		static double fade(double v);
		static double smooth_lerp(double x, double y, double s);

	private:
#if !USE_PSEUDO
		static random_device device;
		static const uchar PERLIN_HASH[];
#endif
	};

}

#endif