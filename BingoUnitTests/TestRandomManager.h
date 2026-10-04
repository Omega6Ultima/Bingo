#pragma once

#include "pch.h"
#include "RandomManager.h"
#include "Surface.h"
#include "WindowManager.h"

TEST(TestRandomManager, TestPerlinNoise) {
	auto& randMan = Bingo::RandomManager::getSingleton();
	auto s = Bingo::Surfaces::Surface(90, 90);

	s.fetchPixels();

	int w = s.getWidth();
	int h = s.getHeight();

	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			double noiseVal = 0.0;
			
			EXPECT_NO_THROW(noiseVal = randMan.perlinNoise2D(x, y, 5, 1.5));

			s.setPixelAt(x, y, Bingo::Colors::WHITE * noiseVal);
		}
	}

	s.releasePixels();

	s.setScale(3);

	Bingo::Surfaces::WindowManager::getSingleton().draw(s, 50, 50);
}