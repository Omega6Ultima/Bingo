//Dustin Gehm
//A subclass of Surface meant for animated sprites
//Clips are used to define the frames of animation in order

#pragma once
#ifndef _ANIMSURFACE_H
#define _ANIMSURFACE_H

#include "Surface.h"

class AnimSurface : public Surface{
public:
	AnimSurface(string path);
	AnimSurface(string path, Color colorKey);
	~AnimSurface();

	/*updates the AnimSurface, advancing the frame counter if necessary*/
	void update();

	/*Defines one clip of the animated sprite*/
	uint addClip(uint x, uint y, uint w, uint h);
	/*Does nothing for AnimSurfaces*/
	void delClip(uint index) = delete;
	void setClip(uint index);
	/*Does nothing for AnimSurfaces*/
	void clearClip() = delete;

	/*Sets the animation speed, this is how many update calls the current frame will last*/
	void setAnimSpeed(uint speed);
	/*Returns the current animation speed*/
	inline uint getAnimSpeed() const{
		return animSpeed;
	}

	void advanceFrame();
	void reverseFrame();

private:
	uint numFrames = 0;
	uint curFrame = 0;
	uint animSpeed = 1;
};

#endif