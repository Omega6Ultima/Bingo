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
			Quaternion(VecN<double, 3> axis, double degrees);

			static Quaternion identity();

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

			double magnitude() const;
			double magnitudeSq() const;
			//returns a quaternion representing that opposite angular rotation
			Quaternion conjugate() const;
			Quaternion inverse() const;
			double dot(const Quaternion& other) const;
			Quaternion operator-() const;
			Quaternion operator-(const Quaternion& other) const;
			//This operator uses the alternative multiplication
			//Ex. q^i * p * q
			Quaternion operator*(const Quaternion& other) const;
			Quaternion operator*(double other) const;
			Quaternion operator/(double other) const;

			inline Quaternion operator/(const Quaternion& other) const {
				return operator*(other.inverse());
			}

			bool operator==(const Quaternion& other) const;

		private:
			VecN<double, 4> values;
		};

		Quaternion pow(const Quaternion& q, uint p);
		Quaternion slerp(const Quaternion& start, const Quaternion& end, double lerp);
		Quaternion squad(const Quaternion& q0, const Quaternion& q1, const Quaternion& q2, const Quaternion& q3, double lerp);

		ostream& operator<<(ostream& os, const Quaternion& q);
	}
}

#endif