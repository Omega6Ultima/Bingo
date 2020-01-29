//Dustin Gehm

#include "RandomManager.h"

#include "ThreadManager.h"

#if USE_PSEUDO
#include <stdlib.h>
#include <time.h>

using std::rand;
using std::srand;
#endif

RandomManager::RandomManager(){
	ATOMIC_LOCK(ThreadManager::randLock);

#if USE_PSEUDO
	srand((unsigned int)time(NULL));
#endif

	ATOMIC_UNLOCK(ThreadManager::randLock);
}

RandomManager::~RandomManager(){
	//
}

int RandomManager::randPosNeg(){
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

bool RandomManager::randBool(){
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

int RandomManager::randInt(int min, int max){
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

float RandomManager::randFloat(float min, float max){
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

double RandomManager::randDouble(double min, double max){
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

#if !USE_PSEUDO
random_device RandomManager::device;
#endif