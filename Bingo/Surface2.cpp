//Dustin Gehm

#if 0

#include <SDL_image.h>

#include "Surface2.h"
#include "WindowManager.h"

Surface2::Surface2(string path){
	SDL_Surface* surf = IMG_Load(path.c_str());

	if (!surf){
		Error("Failed to create surface2", IMG_GetError());
	}

	surface = surf;
	width = surf->w;
	height = surf->h;
}

Surface2::~Surface2(){
	SDL_FreeSurface(surface);
}

uint Surface2::getWidth() const{
	return width;
}

uint Surface2::getHeight() const{
	return height;
}

void Surface2::draw(const Surface2& surf, int x, int y){
	//if (WindowManager::getSingleton().curRenderTarget != this){
	//	Warn("The target isnt the render target\n");
	//}

	SDL_Rect dest = { x, y, static_cast<int>(surf.getWidth()), static_cast<int>(surf.getHeight()) };

	SDL_BlitSurface(surf.surface, NULL, surface, &dest);
}

#endif