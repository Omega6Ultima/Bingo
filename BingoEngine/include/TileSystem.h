#pragma once
#ifndef _TILE_SYSTEM_H
#define _TILE_SYSTEM_H

#include <string>

#include "CoordMap.h"
#include "PhysicalObject.h"
#include "Surface.h"

using std::string;

namespace Bingo {

	namespace Tiling {

		using Math::PhysicalObject;
		using Math::VecN;
		using Surfaces::Surface;

		class TileSystem : public PhysicalObject {
		public:
			TileSystem(string mapFile, string tileSet, vector<VecN<int, 4>> tiles);
			~TileSystem();

			inline uint getWidth() const {
				return mapWidth * tileSize[0];
			}

			inline uint getHeight() const {
				return mapHeight * tileSize[1];
			}

			void render(Surface& screen);

		private:
			void parseMapFile(string mapFile);

		private:
			map<VecN<int, 2>, uint> map;
			uint mapWidth = 0;
			uint mapHeight = 0;
			Surface tiles;
			VecN<int, 2> tileSize;
			bool dirty = true;
			Surface renderSurf;
		};

	}

}

#endif
