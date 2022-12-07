//Dustin Gehm
/*A window class used to encapsulate the virtual screen
and the renderer used to draw to it*/

#pragma once
#ifndef _WINDOW_H
#define _WINDOW_H

#include <queue>
#include <string>

#include <SDL_render.h>
#include <SDL_video.h>

#include "Color.h"
#include "Singleton.h"
#include "Timer.h"

using std::queue;
using std::string;

#define FPS_BUF_SIZE 60

namespace Bingo {

	namespace Surfaces {

		using Colors::Color;
		using Time::Timer;

		class Surface;

		class WindowManager :public Singleton<WindowManager> {
		public:
			WindowManager(string title, int xpos, int ypos, int width, int height);
			~WindowManager();

			/*return the width of the window*/
			inline int getWidth() const {
				return width;
			}
			/*return the height of the window*/
			inline int getHeight() const {
				return height;
			}

			/*sets the color used for clearing*/
			void setDrawColor(Color col);
			/*returns the draw color of the window*/
			inline const Color& getDrawColor() const {
				return drawColor;
			}

			uint getFPS();

			SDL_Renderer* getRenderer() const {
				return renderer;
			}

			Uint32 getPixelFormat() const;

			void toggleFullscreen();
			bool getFullscreen() const {
				return fullscreen;
			}

			//void setRenderTarget(Surface* surf);

			/*clears the window with the current draw color*/
			void clear();
			/*copies the surface onto the window at (x, y)*/
			void draw(Surface& surf, int x, int y);
			/*actually draws whatever has been drawn to the screen*/
			void update();
		private:
			SDL_Window* window = NULL;
			SDL_Renderer* renderer = NULL;
			Surface* curRenderTarget = NULL;
			Color drawColor = Colors::WHITE;
			int width = 1, height = 1;
			bool fullscreen = false;

			Timer timer;
			uint culTime = 1;
			queue<uint> drawTimes;

			friend class Surface;
		};

	}

}
#endif