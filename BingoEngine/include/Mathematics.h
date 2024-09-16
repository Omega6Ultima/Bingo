//Dustin Gehm

#pragma once
#ifndef _MATHEMATICS_H
#define _MATHEMATICS_H

#include "VecN.h"
#include "Utils.h"

namespace Bingo {

	namespace Math {

		using Bingo::Utils::degToRadians;

		//distance squared between 2 points
		template<class T>
		float DistanceSq(const T& p1, const T& p2) {
			float diffX = static_cast<float>(p2.getX() - p1.getX());
			float diffY = static_cast<float>(p2.getY() - p1.getY());

			return diffX * diffX + diffY * diffY;
		}

		//distance between 2 points
		template<class T>
		double Distance(const T& p1, const T& p2) {
			double diffX = p2[0] - p1[0];
			double diffY = p2[1] - p1[1];

			return sqrt(diffX * diffX + diffY * diffY);
		}

		template<class T>
		double TriangleArea(T& p1, T& p2, T& p3) {
			double area =
				(p2.getX() * p1.getY() - p1.getX() * p2.getY()) +
				(p3.getX() * p2.getY() - p2.getX() * p3.getY()) +
				(p1.getX() * p3.getY() - p3.getX() * p1.getY());

			area = abs(area / 2);

			return area;
		}

		template<class T>
		double RectArea(T& p1, T& p2, T& p3, T& p4) {
			double length = Distance(p1, p2);
			double width = Distance(p2, p3);

			return length * width;
		}

		template<class T>
		VecN<T, 2> RotatePoint(VecN<T, 2> point, VecN<T, 2> origin, int degrees) {
			double radians = degToRadians(degrees);
			double angleS = sin(radians);
			double angleC = cos(radians);

			double x = angleC * (point.getX() - origin.getX()) - angleS * (point.getY() - origin.getY()) + origin.getX();
			double y = angleS * (point.getX() - origin.getX()) + angleC * (point.getY() - origin.getY()) + origin.getY();

			return { static_cast<T>(x), static_cast<T>(y) };
		}

		template<class T>
		bool PointRectIntersect(T& p, T& r1, T& r2, T& r3, T& r4) {
			double triArea =
				TriangleArea(r1, r2, p) +
				TriangleArea(r2, r3, p) +
				TriangleArea(r3, r4, p) +
				TriangleArea(r1, r4, p);

			double rectArea = RectArea(r1, r2, r3, r4);

			if (triArea > rectArea) {
				return false;
			}

			return true;
		}

		template<class T>
		bool RectRectIntersect(T& r1a, T& r1b, T& r1c, T& r1d, T& r2a, T& r2b, T& r2c, T& r2d) {
			// TODO
			// https://stackoverflow.com/questions/62028169/how-to-detect-when-rotated-rectangles-are-colliding-each-other
			// Combine with PointRectIntersect

			//if (r1a.getX() == r1c.getX() || r1a.getY() == r1c.getY()
			//	|| r2a.getX() == r2c.getX() || r2a.getY() == r1c.getY()) {

			//	return false;
			//}

			//if (r1a.getX() > r2c.getX() || r2a.getX() > r1c.getX()) {
			//	return false;
			//}

			//if (r1c.getY() > r2a.getY() || r2c.getY() > r1a.getY()) {
			//	return false;
			//}

			//return true;

			T rect1Center = (r1a + r1b + r1c + r1d) / 4;
			const T* rect1ClosestPoint = NULL;
			float rect1ClosestDist = FLT_MAX;

			T rect2Center = (r2a + r2b + r2c + r2d) / 4;
			const T* rect2ClosestPoint = NULL;
			float rect2ClosestDist = FLT_MAX;

			for (auto& p : { r1a, r1b, r1c, r1d }) {
				float dist = DistanceSq(p, rect2Center);

				if (dist < rect1ClosestDist) {
					rect1ClosestDist = dist;
					rect1ClosestPoint = &p;
				}
			}

			for (auto& p : { r2a, r2b, r2c, r2d }) {
				float dist = DistanceSq(p, rect1Center);

				if (dist < rect2ClosestDist) {
					rect2ClosestDist = dist;
					rect2ClosestPoint = &p;
				}
			}

			float dist1 = DistanceSq(rect1Center, *rect1ClosestPoint);
			float dist2 = DistanceSq(rect1Center, *rect2ClosestPoint);

			float dist3 = DistanceSq(rect2Center, *rect2ClosestPoint);
			float dist4 = DistanceSq(rect2Center, *rect1ClosestPoint);

			if (dist1 < dist2 || dist3 < dist4) {
				return false;
			}

			return true;
		}

		inline double cotan(double degrees) {
			return 1.0 / std::tan(degToRadians(degrees));
		}

	}

}

#endif