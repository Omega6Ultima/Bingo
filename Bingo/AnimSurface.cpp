//Dustin Gehm

#include "AnimSurface.h"

AnimSurface::AnimSurface(string path)
: Surface(path){
	//
}

AnimSurface::AnimSurface(string path, Color colorKey)
: Surface(path, colorKey){
	//
}

AnimSurface::~AnimSurface(){
	//
}

void AnimSurface::update(){
	if (animSpeed > 0){
		uint clipIndex = (curFrame / animSpeed) % numFrames;

		activeClip = clips[clipIndex];
		curFrame++;
	}
	else{
		activeClip = clips[curFrame];
	}
}

uint AnimSurface::addClip(uint x, uint y, uint w, uint h){
	clips[numFrames] = new SDL_Rect{ static_cast<int>(x), static_cast<int>(y), 
			static_cast<int>(w), static_cast<int>(h) };
	numFrames++;

	return 0;
}

void AnimSurface::setClip(uint index){
	Surface::setClip(index);

	curFrame = index;
}

void AnimSurface::setAnimSpeed(uint speed){
	animSpeed = speed;
}

void AnimSurface::advanceFrame(){
	if (animSpeed > 0){
		curFrame = ((curFrame / animSpeed) + 1) % numFrames;
	}
	else{
		curFrame = (curFrame + 1) % numFrames;
	}
}

void AnimSurface::reverseFrame(){
	if (animSpeed > 0){
		curFrame = ((curFrame / animSpeed) - 1) % numFrames;
	}
	else{
		curFrame = (curFrame - 1) % numFrames;
	}
}