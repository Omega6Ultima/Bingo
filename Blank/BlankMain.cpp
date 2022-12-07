//Dustin Gehm

#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Color.h"
#include "EventManager.h"
#include "Utils.h"
#include "Surface.h"
#include "WindowManager.h"

using namespace std;

using Bingo::Colors::BLACK;
using Bingo::EventManager;
using Bingo::Events::QuitListener;
using Bingo::Surfaces::Surface;
using Bingo::Surfaces::WindowManager;

#ifndef MIXER_AUDIO_FREQ
#define MIXER_AUDIO_FREQ 44100
#endif

#ifndef MIXER_AUDIO_CHANNELS
#define MIXER_AUDIO_CHANNELS 2
#endif

#define PROJ_NAME "Blank"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_TIMERS 18

void initSDLModules() {
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

	if (Mix_OpenAudio(MIXER_AUDIO_FREQ, MIX_DEFAULT_FORMAT, MIXER_AUDIO_CHANNELS, 2048) < 0) {
		cerr << "SDL_mixer could not be loaded\n";
		cerr << Mix_GetError();
		BAIL(1);
	}
}

void quitSDLModules() {
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	initSDLModules();

	WindowManager windowManager(PROJ_NAME, 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT);
	EventManager eventManager;
	QuitListener quitListener;

	Surface screen(SCREEN_WIDTH, SCREEN_HEIGHT);

	while (!quitListener.getDone()) {
		eventManager.update();

		screen.setRenderTarget();
		screen.fill(BLACK);

		// Draw to screen here

		screen.clearRenderTarget();

		windowManager.draw(screen, 0, 0);

		windowManager.update();
	}

	quitSDLModules();

	return 0;
}