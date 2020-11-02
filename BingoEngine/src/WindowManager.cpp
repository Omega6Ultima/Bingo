//Dustin Gehm

#include "WindowManager.h"

#include "Surface.h"

using Bingo::Surfaces::Surface;
using Bingo::Surfaces::WindowManager;
using Bingo::Utils::Error;

WindowManager::WindowManager(string title, int xpos, int ypos, int width, int height) {
	window = SDL_CreateWindow(title.c_str(), xpos, ypos, width, height, SDL_WINDOW_SHOWN);

	if (!window) {
		Error("Failed to create window.", SDL_GetError());
	}

	uint rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE;
	renderer = SDL_CreateRenderer(window, -1, rendererFlags);

	if (!renderer) {
		Error("Failed to create renderer for window.", SDL_GetError());
	}

	SDL_SetRenderDrawColor(renderer, drawColor.getRed(), drawColor.getGreen(), drawColor.getBlue(), drawColor.getAlpha());
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	this->width = width;
	this->height = height;
}

WindowManager::~WindowManager() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

uint WindowManager::getFPS() {
	return (uint)(FPS_BUF_SIZE / (culTime / 1000.0f));
}

Uint32 WindowManager::getPixelFormat() const {
	return SDL_GetWindowPixelFormat(window);
}

void WindowManager::setDrawColor(Color col) {
	drawColor = col;

	SDL_SetRenderDrawColor(renderer, drawColor.getRed(), drawColor.getGreen(), drawColor.getBlue(), drawColor.getAlpha());
}

void WindowManager::clear() {
	SDL_RenderClear(renderer);
}

void WindowManager::draw(Surface& surf, int x, int y) {
	SDL_Rect dest = { x, y, surf.getWidth(), surf.getHeight() };

	if (surf.isDirty()) {
		surf.renderTexture();
	}

	if (surf.activeClip) {
		dest.w = surf.activeClip->w;
		dest.h = surf.activeClip->h;
	}

	SDL_RenderCopyEx(renderer, surf.texture, surf.activeClip, &dest, surf.rotation, &surf.sdl_center, surf.flipState);
}

void WindowManager::update() {
	timer.end();
	timer.start();

	drawTimes.push(timer.diff());
	culTime += drawTimes.back();

	if (drawTimes.size() > FPS_BUF_SIZE) {
		culTime -= drawTimes.front();
		drawTimes.pop();
	}

	SDL_RenderPresent(renderer);
}