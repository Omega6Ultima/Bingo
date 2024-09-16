#pragma once
#ifndef BINGO_CORE_H
#define BINGO_CORE_H

#include <mutex>

#include "Singleton.h"

namespace Bingo {

	namespace Core {

		using std::mutex;

		class BingoEngine : public Singleton<BingoEngine> {
		public:
			void startEngine();
			void stopEngine();

			inline bool isLoaded() const {
				return engineLoaded;
			}

		private:
			bool engineLoaded = false;
		};

		class Manager {
		public:
			Manager();
			~Manager();
		private:
			static uint managerCount;
			mutex managerCountLock;
		};
	}
}

#endif