//

#pragma once
#ifndef _RECT_H
#define _RECT_H

#include <array>

#include <SDL_rect.h>

namespace Bingo {

	class Rect {
	public:
		Rect(int x, int y, int w, int h);
		Rect(const Rect& other);

		void setX(int x);
		inline int getX() const {
			return values[0];
		}

		void setY(int y);
		inline int getY() const {
			return values[1];
		}

		void setW(int w);
		inline int getW() const {
			return values[2];
		}

		void setH(int h);
		inline int getH() const {
			return values[3];
		}

	private:
		std::array<int, 4> values;
	};
}

#endif