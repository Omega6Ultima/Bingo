//Dustin Gehm

#pragma once
#ifndef _THREAD_MANAGER_H
#define _THREAD_MANAGER_H

#include <map>

#include <SDL_thread.h>

#include "Core.h"
#include "NBT.h"
#include "Singleton.h"

using std::map;

namespace Bingo {

	namespace InternalUse {

		static int anotherLayerOfAbstraction(void* data);

	}

	using Core::Manager;
	using Utils::AtomicLock;

	typedef void(*ThreadFunc)(NBT_Compound*);

	class ThreadManager : public Singleton<ThreadManager>, public Manager {
	private:
		struct Thread {
			Thread() {
				name = "";
			}

			Thread(string threadName, NBT_Compound* compound, ThreadFunc function, float delaySeconds) {
				name = threadName;
				nbt = compound;
				func = function;
				delay = delaySeconds;
			}

			string name;
			NBT_Compound* nbt = NULL;
			ThreadFunc func = NULL;
			float delay = 0.0f;
		};
	public:
		ThreadManager();
		~ThreadManager();

		void createThread(string threadName, NBT_Compound* nbt, ThreadFunc function, float delaySeconds = 0.0f);
		void deleteThread(string threadName);
		void startThread(string threadName);
		void waitOnThread(string threadName);

		static AtomicLock fileLock;
		static AtomicLock threadLock;
		static AtomicLock coutLock;
		static AtomicLock randLock;

	private:
		map<string, Thread> threads;
		map<string, SDL_Thread*> sdl_threads;

		friend int InternalUse::anotherLayerOfAbstraction(void* data);
	};

}

#endif