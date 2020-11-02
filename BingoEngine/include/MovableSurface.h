//Dustin Gehm

#pragma once
#ifndef _CONTROLSURFACE_H
#define _CONTROLSURFACE_H

#include "Color.h"
#include "PhysicalObject.h"
#include "Surface.h"
#include "Timer.h"
#include "VecN.h"

namespace Bingo {

	namespace Surfaces {
		using Colors::Color;
		using Math::PhysicalObject;
		using Math::VecN;
		using Time::Timer;

		class MovableSurface : public Surface, public PhysicalObject {
		public:
			enum MoveDir { MOV_UP, MOV_DOWN, MOV_LEFT, MOV_RIGHT };

			MovableSurface(uint width, uint height, int posX, int posY, float forceX, float forceY);
			MovableSurface(string path, int posX, int posY, float forceX, float forceY);
			MovableSurface(string path, Color colorKey, int x, int y, float forceX, float forceY);
			~MovableSurface();

			void move(MoveDir dir);
			void update();

		private:
			Timer timer;
			VecN<float, 2> force;
		};

	}

}

#endif