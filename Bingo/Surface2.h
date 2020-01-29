//Dustin Gehm

#pragma once
#ifndef _SURFACE2_H
#define _SURFACE2_H

#include <string>

#include <SDL_render.h>

#include "Color.h"

using std::string;

class Surface2{
public:
	Surface2(string path);
	//Surface2(string path, Color colorKey);
	//Surface2(int width, int height);
	~Surface2();

	uint getWidth() const;
	uint getHeight() const;

	void draw(const Surface2& surf, int x, int y);
protected:
	//
private:
	SDL_Surface* surface;
	uint width = 1;
	uint height = 1;
};

#endif