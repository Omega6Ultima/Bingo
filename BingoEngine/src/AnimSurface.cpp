//Dustin Gehm

#include "AnimSurface.h"

using Bingo::Surfaces::AnimSurface;

AnimSurface::AnimSurface(string path)
	: Surface(path) {
	//
}

AnimSurface::AnimSurface(string path, Color colorKey)
	: Surface(path, colorKey) {
	//
}

AnimSurface::~AnimSurface() {
	//
}

void AnimSurface::update(uint times) {
	if (animSpeed > 0) {
		uint clipIndex = (frameCounter / animSpeed) % numFrames;

		//activeClip = clips[clipIndex];
		Surface::setClip(clipIndex);
		frameCounter = (frameCounter + times) % (numFrames * animSpeed);
	}
	else {
		//activeClip = clips[frameCounter];
		Surface::setClip(frameCounter);
	}
}

uint AnimSurface::addClip(uint x, uint y, uint w, uint h) {
	//clips[numFrames] = new SDL_Rect{ static_cast<int>(x), static_cast<int>(y),
	//		static_cast<int>(w), static_cast<int>(h) };
	numFrames++;

	return Surface::addClip(x, y, w, h);
}

void AnimSurface::setClip(uint index) {
	Surface::setClip(index);

	frameCounter = index;
}

void AnimSurface::setAnimSpeed(uint speed) {
	animSpeed = speed;
}

void AnimSurface::advanceFrame() {
	if (animSpeed > 0) {
		frameCounter = (frameCounter + animSpeed) % numFrames;
	}
	else {
		frameCounter = (frameCounter + 1) % numFrames;
	}
}

void AnimSurface::reverseFrame() {
	if (animSpeed > 0) {
		frameCounter = (frameCounter - animSpeed) % numFrames;
	}
	else {
		frameCounter = (frameCounter - 1) % numFrames;
	}
}