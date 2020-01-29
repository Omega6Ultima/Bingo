//Dustin Gehm

#include "ThreadManager.h"

#include "Timer.h"

#define DELAYS_ARE_WORKING 0

ThreadManager::ThreadManager(){
	//
}

ThreadManager::~ThreadManager(){
	for (auto iter = sdl_threads.begin(); iter != sdl_threads.end(); iter++){
		SDL_WaitThread(iter->second, NULL);
	}
}

void ThreadManager::createThread(string threadName, NBT_Compound* nbt, ThreadFunc function, float delaySeconds){
	threads[threadName] = Thread(threadName, nbt, function, delaySeconds);
}

void ThreadManager::deleteThread(string threadName){
	threads.erase(threadName);
}

void ThreadManager::startThread(string threadName){
	if (threads.find(threadName) != threads.end()){
		sdl_threads[threadName] = SDL_CreateThread(anotherLayerOfAbstraction, threadName.c_str(), &threads[threadName]);
	}
}

void ThreadManager::waitOnThread(string threadName){
	if (threads.find(threadName) != threads.end()){
		SDL_WaitThread(sdl_threads[threadName], NULL);
	}
}

static int anotherLayerOfAbstraction(void* data){
	//start of threaded code
	ThreadManager::Thread* thread = ((ThreadManager::Thread*)data);

#if DELAYS_ARE_WORKING
	DelayMS(sec_to_ms(thread->delay));
#endif

	thread->func(thread->nbt);

	ATOMIC_LOCK(ThreadManager::threadLock);

	ThreadManager::getSingleton().sdl_threads.erase(thread->name);

	ATOMIC_UNLOCK(ThreadManager::threadLock);

	return 0;
}

AtomicLock ThreadManager::fileLock = 0;
AtomicLock ThreadManager::threadLock = 0;
AtomicLock ThreadManager::coutLock = 0;
AtomicLock ThreadManager::randLock = 0;







