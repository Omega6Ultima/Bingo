//Dustin Gehm

#include <iostream>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Color.h"
#include "EventManager.h"
#include "FontManager.h"
#include "Gui.h"
#include "Matrix.h"
#include "Surface.h"
#include "Utils.h"
#include "WindowManager.h"

using namespace std;

using Bingo::Colors::Color;
using Bingo::EventManager;
using Bingo::Events::QuitListener;
using Bingo::Events::MouseListener;
using Bingo::FontManager;
using Bingo::Guis::Button;
using Bingo::Guis::ButtonText;
using Bingo::Math::Matrix;
using Bingo::Surfaces::Surface;
using Bingo::Surfaces::WindowManager;

#ifndef MIXER_AUDIO_FREQ
#define MIXER_AUDIO_FREQ 44100
#endif

#ifndef MIXER_AUDIO_CHANNELS
#define MIXER_AUDIO_CHANNELS 2
#endif

#define PROJ_NAME "BS"
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

void M_BoardButtonClicked(Button& button, EventManager::MouseButton mouseButton) {
	Surface::saveRenderTarget();

	button.setRenderTarget();

	button.setDrawColor(Bingo::Colors::RED);
	button.drawCircle(15, 15, 5, true);

	button.clearRenderTarget();

	Surface::restoreRenderTarget();
}

void E_BoardButtonClicked(Button& button, EventManager::MouseButton mouseButton) {
	Surface::saveRenderTarget();

	button.setRenderTarget();

	button.setDrawColor(Bingo::Colors::BLUE);
	button.drawCircle(15, 15, 5, true);

	button.clearRenderTarget();

	Surface::restoreRenderTarget();
}

Button* draggingShip = NULL;

void DragShip(Button& button, EventManager::MouseButton mouseButton) {
	if (mouseButton == EventManager::MouseButton::MB_LEFT) {
		draggingShip = &button;
	}
	else if (mouseButton == EventManager::MouseButton::MB_RIGHT) {
		button.setRotation(button.getRotation() + 90);

		//TODO rotating buttons doesnt rotate the hitzone
	}
}

int main(int argc, char* argv[]) {
	initSDLModules();

	WindowManager windowManager(PROJ_NAME, 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT);
	EventManager eventManager;
	QuitListener quitListener;
	MouseListener mouseListener;
	FontManager fontManager("./resources/fonts/");

	Bingo::Events::KeyListener keyListener;

	Button carrierSurf(50, 400, DragShip, "./resources/images/Sea Warfare Set/Carrier/ShipCarrierHull.png");
	Button battleshipSurf(125, 400, DragShip, "./resources/images/Sea Warfare Set/Battleship/ShipBattleshipHull.png");
	Button cruiserSurf(175, 400, DragShip, "./resources/images/Sea Warfare Set/Cruiser/ShipCruiserHull.png");
	Button submarineSurf(225, 400, DragShip, "./resources/images/Sea Warfare Set/Submarine/ShipSubMarineHull.png");
	Button patrolSurf(275, 400, DragShip, "./resources/images/Sea Warfare Set/PatrolBoat/ShipPatrolHull.png");
	//TODO if these are declared after screen, it messes with the screen

	carrierSurf.setScaleX(.5f);
	carrierSurf.setScaleY(.7f);
	battleshipSurf.setScale(.5f);
	cruiserSurf.setScale(.6f);
	submarineSurf.setScale(.5f);
	patrolSurf.setScale(.7f);

	Surface background("./resources/images/Background.png");
	Surface screen(SCREEN_WIDTH, SCREEN_HEIGHT);

	fontManager.nickname("expressway rg.ttf", "main");

	Matrix<char, 10, 10> mShips;
	Matrix<char, 10, 10> mShots;
	Matrix<char, 10, 10> eBoard;
	Matrix<char, 10, 10> eShots;

	vector<Button*> mBoardButtons;
	vector<Button*> eBoardButtons;

	for (size_t i = 0; i < 10; i++) {
		for (size_t j = 0; j < 10; j++) {
			Button* mButton = new Button(50 + i * 30, 50 + j * 30, M_BoardButtonClicked, 30, 30);
			Button* eButton = new Button(450 + i * 30, 50 + j * 30, E_BoardButtonClicked, 30, 30);

			Surface::saveRenderTarget();

			mButton->setRenderTarget();
			mButton->setDrawColor(Bingo::Colors::LIGHTGRAY);
			mButton->drawRect(0, 0, 30, 30, true);
			mButton->setDrawColor(Bingo::Colors::WHITE);
			mButton->drawCircle(15, 15, 5, true);

			eButton->setRenderTarget();
			eButton->setDrawColor(Bingo::Colors::DARKGRAY);
			eButton->drawRect(0, 0, 30, 30, true);
			eButton->setDrawColor(Bingo::Colors::WHITE);
			eButton->drawCircle(15, 15, 5, true);

			Surface::restoreRenderTarget();

			mBoardButtons.push_back(mButton);
			eBoardButtons.push_back(eButton);
		}
	}

	enum GamePhase {
		SETUP,
		PLAY,
	} gamePhase = SETUP;

	//disable all board buttons
	for (auto& button : mBoardButtons) {
		button->disable();
	}

	for (auto& button : eBoardButtons) {
		button->disable();
	}

	while (!quitListener.getDone()) {
		eventManager.update();

		if (gamePhase == SETUP) {
			if (draggingShip) {
				draggingShip->setPosX(mouseListener.getMousePos()[0] - (draggingShip->getWidth() / 2));
				draggingShip->setPosY(mouseListener.getMousePos()[1] - (draggingShip->getHeight() / 2));
			}

			if (draggingShip && !mouseListener.checkMouseButtonDown(EventManager::MouseButton::MB_LEFT, 0)) {
				draggingShip = NULL;
			}

			if (keyListener.checkKeyDown(EventManager::KeyCode::K_UP)) {
				cout << carrierSurf.getScaleX() + .05f << endl;

				carrierSurf.setScale(carrierSurf.getScaleX() + .05f);
				battleshipSurf.setScale(battleshipSurf.getScaleX() + .05f);
				cruiserSurf.setScale(cruiserSurf.getScaleX() + .05f);
				submarineSurf.setScale(submarineSurf.getScaleX() + .05f);
				patrolSurf.setScale(patrolSurf.getScaleX() + .05f);
			}
			else if (keyListener.checkKeyDown(EventManager::KeyCode::K_DOWN)) {
				cout << carrierSurf.getScaleX() - .05f << endl;

				carrierSurf.setScale(carrierSurf.getScaleX() - .05f);
				battleshipSurf.setScale(battleshipSurf.getScaleX() - .05f);
				cruiserSurf.setScale(cruiserSurf.getScaleX() - .05f);
				submarineSurf.setScale(submarineSurf.getScaleX() - .05f);
				patrolSurf.setScale(patrolSurf.getScaleX() - .05f);
			}

			//rotate ship on right click
		}
		else if (gamePhase == PLAY) {
			//
		}

		screen.setRenderTarget();

		screen.draw(background);

		for (auto& button : mBoardButtons) {
			screen.draw(*button);
		}

		for (auto& button : eBoardButtons) {
			screen.draw(*button);
		}

		//TODO maybe draw evenly spaced method
		screen.draw(carrierSurf);
		screen.draw(battleshipSurf);
		screen.draw(cruiserSurf);
		screen.draw(submarineSurf);
		screen.draw(patrolSurf);

		screen.clearRenderTarget();
		windowManager.draw(screen, 0, 0);
		windowManager.update();
	}

	for (auto& button : mBoardButtons) {
		delete button;
	}

	for (auto& button : eBoardButtons) {
		delete button;
	}

	quitSDLModules();

	return 0;
}