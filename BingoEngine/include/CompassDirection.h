//Dustin Gehm

#pragma once
#ifndef _COMPASSDIRECTION_H
#define _COMPASSDIRECTION_H

#include "VecN.h"

namespace Bingo {

	namespace Direction {

		using Math::VecN;

		class CompassDirection {
		public:
			CompassDirection();
			CompassDirection(char dir);
			CompassDirection(char dir, char dir2);
			CompassDirection(VecN<float, 2> vec);
			~CompassDirection();

			VecN<int, 2> getMultipliers(int defaultVal = 0) const;

			CompassDirection operator +(const CompassDirection& cd) const;
			CompassDirection operator -(const CompassDirection& cd) const;

			void operator +=(const CompassDirection& cd);
			void operator -=(const CompassDirection& cd);

		private:
			char vertical;
			char horizontal;
		};

		const CompassDirection NORTH('N');
		const CompassDirection EAST('E');
		const CompassDirection SOUTH('S');
		const CompassDirection WEST('W');
		const CompassDirection NORTHEAST('N', 'E');
		const CompassDirection SOUTHEAST('S', 'E');
		const CompassDirection SOUTHWEST('S', 'W');
		const CompassDirection NORTHWEST('N', 'W');

	}

}

#endif