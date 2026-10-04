//Dustin Gehm

#include "RandomManager.h"

#include "ThreadManager.h"

#if USE_PSEUDO
#include <stdlib.h>
#include <time.h>

using std::rand;
using std::srand;
#endif

using Bingo::RandomManager;

RandomManager::RandomManager() {
	ATOMIC_LOCK(ThreadManager::randLock);

#if USE_PSEUDO
	srand((unsigned int)time(NULL));
#endif

	ATOMIC_UNLOCK(ThreadManager::randLock);
}

RandomManager::~RandomManager() {
	//
}

int RandomManager::randPosNeg() {
	int result;

	ATOMIC_LOCK(ThreadManager::randLock);

#if USE_PSEUDO
	result = rand() % 2 == 0 ? -1 : 1;
#else
	result = device() % 2 == 0 ? -1 : 1;
#endif

	ATOMIC_UNLOCK(ThreadManager::randLock);

	return result;
}

bool RandomManager::randBool() {
	bool result;

	ATOMIC_LOCK(ThreadManager::randLock);

#if USE_PSEUDO
	result = rand() % 2 == 0 ? true : false;
#else
	result = device() % 2 == 0 ? true : false;
#endif

	ATOMIC_UNLOCK(ThreadManager::randLock);

	return result;
}

int RandomManager::randInt(int min, int max) {
	int result;

	ATOMIC_LOCK(ThreadManager::randLock);

#if USE_PSEUDO
	result = min + (rand() % (1 + max - min));
#else
	result = min + (device() % (1 + max - min));
#endif

	ATOMIC_UNLOCK(ThreadManager::randLock);

	return result;
}

float RandomManager::randFloat(float min, float max) {
	float result;

	ATOMIC_LOCK(ThreadManager::randLock);

#if USE_PSEUDO
	result = min + (rand() * (max - min) / RAND_MAX);
#else
	result = min + (device() * (max - min) / device.max());
#endif

	ATOMIC_UNLOCK(ThreadManager::randLock);

	return result;
}

double RandomManager::randDouble(double min, double max) {
	double result;

	ATOMIC_LOCK(ThreadManager::randLock);

#if USE_PSEUDO
	result = min + (rand() * (max - min) / RAND_MAX);
#else
	result = min + (device() * (max - min) / device.max());
#endif

	ATOMIC_UNLOCK(ThreadManager::randLock);

	return result;
}

double RandomManager::perlinNoise(double x, double y, double depth, double frequency, int seed) {
	double modX = x * frequency;
	double modY = y * frequency;
	double amplitude = 1.0;
	double result = 0.0;
	double divisor = 0.0;

	for (int i = 0; i < depth; i++) {
		divisor += 256 * amplitude;

		result += noise(x, y, seed) * amplitude;

		amplitude /= 2.0;
		modX *= 2.0;
		modY *= 2.0;
	}

	return result / divisor;
}

double RandomManager::perlinNoise(double x, double y, double z, double depth, double frequency, int seed) {
	double modX = x * frequency;
	double modY = y * frequency;
	double amplitude = 1.0;
	double result = 0.0;
	double divisor = 0.0;

	for (int i = 0; i < depth; i++) {
		divisor += 256 * amplitude;

		result += noise(x, y, z, seed) * amplitude;

		amplitude /= 2.0;
		modX *= 2.0;
		modY *= 2.0;
	}

	return result / divisor;
}

double RandomManager::noise(double x, double y, int seed) {
	x = fmod(x, 256.0);
	y = fmod(y, 256.0);

	int xFloor = static_cast<int>(std::floor(x)) & 255;
	int yFloor = static_cast<int>(std::floor(y)) & 255;

	double xFrac = x - xFloor;
	double yFrac = y - yFloor;

	int a = hashFunc(xFloor,		yFloor,		seed);
	int b = hashFunc(xFloor + 1,	yFloor,		seed);
	int c = hashFunc(xFloor,		yFloor + 1, seed);
	int d = hashFunc(xFloor + 1,	yFloor + 1, seed);

	return smooth_lerp(
		smooth_lerp(a, b, fade(xFrac)),
		smooth_lerp(c, d, fade(xFrac)),
		fade(yFrac));
}

double RandomManager::noise(double x, double y, double z, int seed) {
	x = fmod(x, 256.0);
	y = fmod(y, 256.0);
	z = fmod(z, 256.0);

	int xFloor = static_cast<int>(std::floor(x)) & 255;
	int yFloor = static_cast<int>(std::floor(y)) & 255;
	int zFloor = static_cast<int>(std::floor(z)) & 255;

	double xFrac = x - xFloor;
	double yFrac = y - yFloor;
	double zFrac = z - zFloor;

	int a = hashFunc(xFloor,		yFloor,			zFloor,		seed);
	int b = hashFunc(xFloor,		yFloor + 1,		zFloor,		seed);
	int c = hashFunc(xFloor,		yFloor,			zFloor + 1,	seed);
	int d = hashFunc(xFloor,		yFloor + 1,		zFloor + 1,	seed);
	int e = hashFunc(xFloor + 1,	yFloor,			zFloor,		seed);
	int f = hashFunc(xFloor + 1,	yFloor + 1,		zFloor,		seed);
	int g = hashFunc(xFloor + 1,	yFloor,			zFloor + 1, seed);
	int h = hashFunc(xFloor + 1,	yFloor + 1,		zFloor + 1,	seed);

	return smooth_lerp(
			smooth_lerp(smooth_lerp(a, b, fade(xFrac)),
						smooth_lerp(c, d, fade(xFrac)),
						fade(yFrac)),
			smooth_lerp(smooth_lerp(e, f, fade(xFrac)),
						smooth_lerp(g, h, fade(xFrac)),
						fade(yFrac)),
			fade(zFrac)
		);
}

int RandomManager::hashFunc(int x, int y, int seed) {
	auto confine_index = [](int num) -> int { if (num < 0) num += 256; return num; };

	return PERLIN_HASH[confine_index((PERLIN_HASH[confine_index((y + seed) % 256)] + x) % 256)];
}

int RandomManager::hashFunc(int x, int y, int z, int seed) {
	auto confine_index = [](int num) -> int { if (num < 0) num += 256; return num; };

	return PERLIN_HASH[confine_index((PERLIN_HASH[confine_index((PERLIN_HASH[confine_index((z + seed) % 256)] + y) % 256)] + x) % 256)];
}

double RandomManager::fade(double v) {
	const double v3 = v * v * v;
	return (6 * v * v * v3) - (15 * v * v3) + (10 * v3);
}

double RandomManager::smooth_lerp(double x, double y, double s) {
	const double s2 = s * s;
	return x + ((-2 * s * s2) + (2 * s2)) * (y - x);
}

#if !USE_PSEUDO
random_device RandomManager::device;
#endif

const uchar RandomManager::PERLIN_HASH[] = {
	208,34,231,213,32,248,233,56,161,78,24,140,71,48,140,254,245,255,247,247,40,
	185,248,251,245,28,124,204,204,76,36,1,107,28,234,163,202,224,245,128,167,204,
	9,92,217,54,239,174,173,102,193,189,190,121,100,108,167,44,43,77,180,204,8,81,
	70,223,11,38,24,254,210,210,177,32,81,195,243,125,8,169,112,32,97,53,195,13,
	203,9,47,104,125,117,114,124,165,203,181,235,193,206,70,180,174,0,167,181,41,
	164,30,116,127,198,245,146,87,224,149,206,57,4,192,210,65,210,129,240,178,105,
	228,108,245,148,140,40,35,195,38,58,65,207,215,253,65,85,208,76,62,3,237,55,89,
	232,50,217,64,244,157,199,121,252,90,17,212,203,149,152,140,187,234,177,73,174,
	193,100,192,143,97,53,145,135,19,103,13,90,135,151,199,91,239,247,33,39,145,
	101,120,99,3,186,86,99,41,237,203,111,79,220,135,158,42,30,154,120,67,87,167,
	135,176,183,191,253,115,184,21,233,58,129,233,142,39,128,211,118,137,139,255,
	114,20,218,113,154,27,127,246,250,1,8,198,250,209,92,222,173,21,88,102,219,

	208,34,231,213,32,248,233,56,161,78,24,140,71,48,140,254,245,255,247,247,40,
	185,248,251,245,28,124,204,204,76,36,1,107,28,234,163,202,224,245,128,167,204,
	9,92,217,54,239,174,173,102,193,189,190,121,100,108,167,44,43,77,180,204,8,81,
	70,223,11,38,24,254,210,210,177,32,81,195,243,125,8,169,112,32,97,53,195,13,
	203,9,47,104,125,117,114,124,165,203,181,235,193,206,70,180,174,0,167,181,41,
	164,30,116,127,198,245,146,87,224,149,206,57,4,192,210,65,210,129,240,178,105,
	228,108,245,148,140,40,35,195,38,58,65,207,215,253,65,85,208,76,62,3,237,55,89,
	232,50,217,64,244,157,199,121,252,90,17,212,203,149,152,140,187,234,177,73,174,
	193,100,192,143,97,53,145,135,19,103,13,90,135,151,199,91,239,247,33,39,145,
	101,120,99,3,186,86,99,41,237,203,111,79,220,135,158,42,30,154,120,67,87,167,
	135,176,183,191,253,115,184,21,233,58,129,233,142,39,128,211,118,137,139,255,
	114,20,218,113,154,27,127,246,250,1,8,198,250,209,92,222,173,21,88,102,219
};