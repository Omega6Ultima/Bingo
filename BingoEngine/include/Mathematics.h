//Dustin Gehm

#pragma once
#ifndef _MATHEMATICS_H
#define _MATHEMATICS_H

#include "VecN.h"

namespace Bingo {

	namespace Math {

		//distance squared between 2 points
		template<class T>
		float DistanceSq(const T& p1, const T& p2) {
			float diffX = static_cast<float>(p2.getX() - p1.getX());
			float diffY = static_cast<float>(p2.getY() - p1.getY());

			return diffX * diffX + diffY * diffY;
		}

		//distance between 2 points
		template<class T>
		float Distance(const T& p1, const T& p2) {
			float diffX = static_cast<float>(p2[0] - p1[0]);
			float diffY = static_cast<float>(p2[1] - p1[1]);

			return sqrt(diffX * diffX + diffY * diffY);
		}

		template<class T>
		float TriangleArea(T& p1, T& p2, T& p3) {
			float area =
				(p2.getX() * p1.getY() - p1.getX() * p2.getY()) +
				(p3.getX() * p2.getY() - p2.getX() * p3.getY()) +
				(p1.getX() * p3.getY() - p3.getX() * p1.getY());

			area = abs(area / 2);

			return area;
		}

		template<class T>
		float RectArea(T& p1, T& p2, T& p3, T& p4) {
			float length = Distance(p1, p2);
			float width = Distance(p2, p3);

			return length * width;
		}

		template<class T>
		bool PointRectIntersect(T& p, T& r1, T& r2, T& r3, T& r4) {
			float triArea =
				TriangleArea(r1, r2, p) +
				TriangleArea(r2, r3, p) +
				TriangleArea(r3, r4, p) +
				TriangleArea(r1, r4, p);

			float rectArea = RectArea(r1, r2, r3, r4);

			if (triArea > rectArea) {
				return false;
			}

			return true;
		}

		//assumes rect = {x, y, w, h}
		template<class T>
		bool RectRectIntersect(T& r1a, T& r1b, T& r1c, T& r1d,
			T& r2a, T& r2b, T& r2c, T& r2d) {

			if (r1a.getX() == r1c.getX() || r1a.getY() == r1c.getY()
				|| r2a.getX() == r2c.getX() || r2a.getY() == r1c.getY()) {

				return false;
			}

			if (r1a.getX() >= r2c.getX() || r2a.getX() >= r1c.getX()) {
				return false;
			}

			if (r1c.getY() >= r2a.getY() || r2c.getY() >= r1a.getY()) {
				return false;
			}

			return true;
		}

	}

}

#endif