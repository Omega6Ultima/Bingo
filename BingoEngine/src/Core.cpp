#include "Core.h"

#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Utils.h"

using Bingo::Core::BingoEngine;
using Bingo::Core::Manager;

using std::cerr;
using std::lock_guard;

void BingoEngine::startEngine() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		cerr << "SDL could not be loaded\n";
		cerr << SDL_GetError();
		BAIL(1);
	}

	uint SDL_IMG_Flags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(SDL_IMG_Flags) & SDL_IMG_Flags)) {
		cerr << "SDL_image could not be loaded\n";
		cerr << IMG_GetError();
		BAIL(1);
	}

	if (TTF_Init() == -1) {
		cerr << "SDL_TTF could not be loaded\n";
		cerr << TTF_GetError();
		BAIL(1);
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		cerr << "SDL_mixer could not be loaded\n";
		cerr << Mix_GetError();
		BAIL(1);
	}

	engineLoaded = true;
}

void BingoEngine::stopEngine() {
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

BingoEngine bingoEngine;

Manager::Manager() {
	auto& eng = BingoEngine::getSingleton();

	lock_guard lg(managerCountLock);

	if (!eng.isLoaded()) {
		eng.startEngine();
	}

	managerCount += 1;
}

Manager::~Manager() {
	lock_guard lg(managerCountLock);

	managerCount -= 1;

	if (managerCount == 0) {
		BingoEngine::getSingleton().stopEngine();
	}
}

uint Manager::managerCount = 0;