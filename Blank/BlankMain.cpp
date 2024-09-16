//Dustin Gehm

#include <iostream>

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

#define PROJ_NAME "Blank"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int argc, char* argv[]) {
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

	return 0;
}