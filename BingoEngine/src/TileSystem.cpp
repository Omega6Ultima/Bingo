#include "TileSystem.h"

#include <fstream>

#include "WindowManager.h"

using std::ifstream;
using std::getline;
using std::streamsize;
using std::stoi;

using Bingo::Tiling::TileSystem;
using Bingo::Surfaces::WindowManager;
using Bingo::Utils::strip;
using Bingo::Utils::tokenize;
using Bingo::Utils::TokenizeType;

TileSystem::TileSystem(string mapFile, string tileSet, vector<VecN<int, 4>> tileClips)
	: tiles(tileSet)
	, renderSurf(WindowManager::getSingleton().getWidth(), WindowManager::getSingleton().getHeight()) {
	tileSize[0] = tileClips[0][2];
	tileSize[1] = tileClips[0][3];

	for (auto iter = tileClips.begin(); iter != tileClips.end(); iter++) {
		tiles.addClip((*iter)[0], (*iter)[1], (*iter)[2], (*iter)[3]);
	}

	parseMapFile(mapFile);
}

TileSystem::~TileSystem() {
	//
}

void TileSystem::render(Surface& screen) {
	if (dirty) {
		renderSurf.saveRenderTarget();
		renderSurf.setRenderTarget();

		renderSurf.clear();

		for (auto iter = map.begin(); iter != map.end(); iter++) {
			tiles.setClip(iter->second);

			renderSurf.draw(tiles, iter->first[0] * tileSize[0], iter->first[1] * tileSize[1]);
		}

		tiles.clearClip();

		renderSurf.restoreRenderTarget();
		dirty = false;
	}

	screen.draw(renderSurf, { this->getPosX(), this->getPosY() });
}

void TileSystem::parseMapFile(string mapFile) {
	ifstream infile(mapFile, std::ios::in);
	string line;
	streamsize numRead = 0;
	int rowInd = 0;
	uint maxCol = 0;

	while (getline(infile, line)) {
		int colInd = 0;
		vector<uint> inds = tokenize(line, TokenizeType::RetVec);

		for (auto iter = inds.begin(); iter != inds.end(); iter++) {
			uint tileNum = 0;
			uint nextSpace = line.find(' ', *iter + 1);
			string numStr = strip(line.substr(*iter, nextSpace - *iter));

			if (numStr.empty()) {
				break;
			}

			tileNum = stoi(numStr);

			try {
				tiles.setClip(tileNum);
			}
			catch (Exception&) {
				throw Exception("Read invalid tile number from file" + numStr);
			}

			map[{colInd, rowInd}] = tileNum;

			maxCol = MAX(maxCol, static_cast<uint>(colInd));

			colInd++;
		}

		rowInd++;
	}

	infile.close();

	mapWidth = maxCol;
	mapHeight = rowInd - 1;
}