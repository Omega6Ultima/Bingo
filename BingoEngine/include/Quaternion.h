//for representing rotations

#pragma once
#ifndef _QUATERNION_H
#define _QUATERNION_H

#include "VecN.h"

namespace Bingo {
	namespace Math {
		class Quaternion {
		public:
			Quaternion();
			Quaternion(double w, double x, double y, double z);
			Quaternion(VecN<double, 3> point);
			//creates a quaternion representing a rotation of angle around axis
			Quaternion(VecN<double, 3> axis, double angle);

			void setW(double w);
			inline double getW() const {
				return values[0];
			}

			void setX(double x);
			inline double getX() const {
				return values[1];
			}

			void setY(double y);
			inline double getY() const {
				return values[2];
			}

			void setZ(double z);
			inline double getZ() const {
				return values[3];
			}

			static Quaternion identity();

			double magnitude() const;
			//returns a quaternion representing that opposite angular rotation
			Quaternion conjugate() const;
			Quaternion inverse() const;
			Quaternion operator-() const;
			Quaternion operator-(const Quaternion& other) const;
			//This operator uses the alternative multiplication
			//Ex. q^i * p * q
			Quaternion operator*(const Quaternion& other) const;
			Quaternion operator*(double other) const;
			Quaternion operator/(double other) const;
			double dot(const Quaternion& other) const;

		private:
			VecN<double, 4> values;
		};

		typedef Quaternion Quat;

		Quaternion pow(const Quaternion& q, double p);
		Quaternion slerp(const Quat& start, const Quat& end, double lerp);
		Quaternion squad(const Quat& q0, const Quat& q1, const Quat& q2, const Quat& q3, double lerp);

		ostream& operator<<(ostream& os, const Quaternion& q);
	}
}

#endif