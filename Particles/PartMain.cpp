//Dustin Gehm

#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Color.h"
#include "EventManager.h"
#include "FontManager.h"
#include "ParticleEngine.h"
#include "RandomManager.h"
#include "Surface.h"
#include "TextSurface.h"
#include "Timer.h"
#include "Utils.h"
#include "VecN.h"
#include "WindowManager.h"

using namespace std;

using Bingo::Colors::BLACK;
using Bingo::Colors::WHITE;
using Bingo::EventManager;
using Bingo::Events::QuitListener;
using Bingo::FontManager;
using Bingo::Math::VecN;
using Bingo::Particles::DefaultBehavior;
using Bingo::Particles::ParticleEngine;
using Bingo::Particles::ParticleEngineBehavior;
using Bingo::RandomManager;
using Bingo::Surfaces::Surface;
using Bingo::Surfaces::TextSurface;
using Bingo::Surfaces::WindowManager;
using Bingo::Time::Timer;

#ifndef MIXER_AUDIO_FREQ
#define MIXER_AUDIO_FREQ 44100
#endif

#ifndef MIXER_AUDIO_CHANNELS
#define MIXER_AUDIO_CHANNELS 2
#endif

#define PROJ_NAME "PARTICLES!"
#define SCREEN_WIDTH 1440
#define SCREEN_HEIGHT 900
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

void PartCallback(Bingo::Particles::Particle& part) {
	//part.markDead();
	//part.setGravityEffects(true);
	part.setVel(part.getVel() * 100.0f);

	if (part.getPosY() > SCREEN_HEIGHT) {
		part.markDead();
	}

	if (part.getPosY() < -SCREEN_HEIGHT) {
		part.markDead();
	}

	if (part.getPosX() > SCREEN_WIDTH) {
		part.markDead();
	}

	if (part.getPosX() < -SCREEN_WIDTH) {
		part.markDead();
	}
}

int main(int argc, char* argv[]) {
	initSDLModules();

	WindowManager windowManager(PROJ_NAME, 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT);
	windowManager.toggleFullscreen();
	EventManager eventManager;
	Bingo::Events::MouseListener mouseListener;
	QuitListener quitListener;
	RandomManager randomManager;
	FontManager fontManager("resources/Fonts/");
	fontManager.nickname("expressway rg.ttf", "main");

	Surface screen(SCREEN_WIDTH, SCREEN_HEIGHT);
	TextSurface fpsSurf("main", 20, "0.0 fps", WHITE);

	ParticleEngine lavaLamp("resources/Images/Lava.png", 300, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, -40, 40, -40, 40);
	ParticleEngine lavaLamp2("resources/Images/BlueLava.png", 375, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, -50, 50, -50, 50);

	Surface purpTri("resources/Images/PurpTri.png");
	//Surface purpTri = Surface(40, 40);
	//purpTri.setBlendMode(Surface::BlendMode::BLEND_BLEND);
	//purpTri.setScale(4.0);
	//purpTri.setRenderTarget();
	//purpTri.clear();
	//purpTri.fill(Bingo::Colors::WHITE);
	//purpTri.setDrawColor(Bingo::Colors::PURPLE);
	//purpTri.drawTriangle(20, 20, 10, 10, 0, true);
	//purpTri.drawCircle(20, 20, 10, true);
	//purpTri.drawRect(10, 10, 20, 20, true);
	//purpTri.clearRenderTarget();

	ParticleEngine lavaLamp3(purpTri, 450, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, -60, 60, -60, 60);
	ParticleEngineBehavior* lavaBehav = new DefaultBehavior();
	Timer forceTimer;

	const uint PartRad = 20;
	const uint PartMinX = 0 + PartRad; // SCREEN_WIDTH / 10;
	const uint PartMaxX = SCREEN_WIDTH - PartRad; // *9 / 10;
	const uint PartMinY = 0 + PartRad; // SCREEN_HEIGHT / 10;
	const uint PartMaxY = SCREEN_HEIGHT - PartRad; // *9 / 10;

	lavaLamp.getParticleSurface().setScale(0.5f);
	lavaLamp2.getParticleSurface().setScale(0.25f);
	lavaLamp3.getParticleSurface().setScale(0.1f);

	lavaBehav->setFriction(0.0005f);
	lavaBehav->setParticleBounds({ PartMinY + PartRad, PartMaxY - (2 * PartRad), PartMinX + PartRad, PartMaxX - 2 * PartRad, PartRad });
	lavaBehav->setBoundBehavior(
		VecN<ParticleEngineBehavior::BoundBehavior, 4> {
		ParticleEngineBehavior::BoundBehavior::BOUNCE,
			ParticleEngineBehavior::BoundBehavior::BOUNCE,
			ParticleEngineBehavior::BoundBehavior::BOUNCE,
			ParticleEngineBehavior::BoundBehavior::BOUNCE,
	});
	//lavaBehav->setBoundBehavior(ParticleEngineBehavior::BoundBehavior::CALLBACK, PartCallback);

	lavaLamp.setBehavior(lavaBehav);
	lavaLamp.remakeParticles();
	lavaLamp.update(2.0f);

	lavaLamp2.setBehavior(lavaBehav);
	lavaLamp2.remakeParticles();
	lavaLamp2.update(2.5f);

	lavaLamp3.setBehavior(lavaBehav);
	lavaLamp3.remakeParticles();
	lavaLamp3.update(3.0f);

	//vector<VecN<int, 2>> boxPoints;
	//boxPoints.push_back({ PartMinX, PartMinY });
	//boxPoints.push_back({ PartMinX, PartMaxY });
	//boxPoints.push_back({ PartMaxX, PartMaxY });
	//boxPoints.push_back({ PartMaxX, PartMinY });
	//boxPoints.push_back({ PartMinX, PartMinY });

	while (!quitListener.getDone()) {
		eventManager.update();

		if (mouseListener.checkMouseButtonDown(Bingo::EventManager::MouseButton::MB_LEFT, 200)) {
			//lavaLamp.addExternalForce(mouseListener.getMousePos(), 50, 100);
			lavaLamp2.addExternalForce(mouseListener.getMousePos(), 50, 100);
			//lavaLamp3.addExternalForce(mouseListener.getMousePos(), 50, 100);
		}

		if (mouseListener.checkMouseButtonDown(Bingo::EventManager::MouseButton::MB_RIGHT, 200)) {
			//lavaLamp.addExternalForce(mouseListener.getMousePos(), 50, 100);
			//lavaLamp2.addExternalForce(mouseListener.getMousePos(), 50, 100);
			lavaLamp3.addExternalForce(mouseListener.getMousePos(), 50, 100);
		}

		if (forceTimer.everyXSeconds(4)) {

			for (uint c = 0; c < 2; c++) {
				VecN<int, 2> impactPos = VecN<int, 2>({ randomManager.randInt(0, SCREEN_WIDTH), randomManager.randInt(0, SCREEN_HEIGHT) });

				lavaLamp.addExternalForce(impactPos, SCREEN_HEIGHT / 4, 5);
				lavaLamp2.addExternalForce(impactPos, SCREEN_HEIGHT / 4, 7.5);
				lavaLamp3.addExternalForce(impactPos, SCREEN_HEIGHT / 4, 10);
			}
		}

		screen.setRenderTarget();
		screen.fill(Bingo::Colors::BLACK);
		screen.setDrawColor(Bingo::Colors::WHITE);
		//screen.drawLines(boxPoints);

		lavaLamp.update();
		lavaLamp2.update();
		lavaLamp3.update();

		lavaLamp.draw(screen);
		lavaLamp2.draw(screen);
		lavaLamp3.draw(screen);

		fpsSurf.setText(to_string(windowManager.getFPS()) + " fps");
		screen.draw(fpsSurf, SCREEN_WIDTH - fpsSurf.getWidth(), SCREEN_HEIGHT - fpsSurf.getHeight());

		screen.clearRenderTarget();


		windowManager.draw(screen, 0, 0);

		windowManager.update();
	}

	quitSDLModules();

	return 0;
}