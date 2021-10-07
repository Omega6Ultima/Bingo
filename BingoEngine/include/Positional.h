//Dustin Gehm

#pragma once
#ifndef _POSITIONAL_H
#define _POSITIONAL_H

#include "VecN.h"

namespace Bingo {

	namespace Math {

		class Positional {
		public:
			Positional();
			Positional(int x, int y);
			Positional(VecN<int, 2> p);
			~Positional();

			void setPos(VecN<int, 2> p);
			inline VecN<int, 2> getPos() const {
				return static_cast<VecN<int, 2>>(pos);
			}

			void setPosX(int x);
			inline int getPosX() const {
				return static_cast<int>(pos[0]);
			}

			void setPosY(int y);
			inline int getPosY() const {
				return static_cast<int>(pos[1]);
			}

		protected:
			VecN<float, 2> pos;
		};

		class Positional3D {
		public:
			Positional3D();
			Positional3D(int x, int y, int z);
			Positional3D(VecN<int, 3> p);
			~Positional3D();

			void setPos(VecN<int, 3> p);
			inline VecN<int, 3> getPos() const {
				return static_cast<VecN<int, 3>>(pos);
			}

			void setPosX(int x);
			inline int getPosX() const {
				return static_cast<int>(pos[0]);
			}

			void setPosY(int y);
			inline int getPosY() const {
				return static_cast<int>(pos[1]);
			}

			void setPosZ(int z);
			inline int getPosZ() const {
				return static_cast<int>(pos[2]);
			}

		protected:
			VecN<float, 3> pos;
		};

	}

}

#endif