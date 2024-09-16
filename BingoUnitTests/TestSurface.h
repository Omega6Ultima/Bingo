#pragma once
#include "pch.h"
#include "Surface.h"
#include "WindowManager.h"

/**
 * images/CyanSquare is a 40x40 Cyan square
 * images/SplitSquare is a 40x40 Red, Yellow, Green, Cyan square
 *		Red is 0,0 to 19,19
 *		Yellow is 20,0 to 39,19
 *		Green is 0,20 to 19,39
 *		Cyan is 20,20 to 39,39
 */

TEST(TestSurface, TestConstruct) {
	auto s = Bingo::Surfaces::Surface("images/CyanSquare.png");
	auto s2 = new Bingo::Surfaces::Surface("images/CyanSquare.png");
	auto other = Bingo::Math::VecN<int, 2>({ 20, 20 });

	EXPECT_EQ(s.getCenter(), other);

	s.fetchPixels();
	EXPECT_EQ(s.getPixelAt(19, 19), Bingo::Colors::CYAN);
	s.releasePixels();

	EXPECT_EQ(s2->getCenter(), other);

	s2->fetchPixels();
	EXPECT_EQ(s2->getPixelAt(19, 19), Bingo::Colors::CYAN);
	s2->releasePixels();

	delete s2;
}

TEST(TestSurface, TestConstruct2) {
	auto s = Bingo::Surfaces::Surface("images/CyanSquare.png", Bingo::Colors::CYAN);
	auto s2 = new Bingo::Surfaces::Surface("images/CyanSquare.png", Bingo::Colors::CYAN);
	auto other = Bingo::Math::VecN<int, 2>({ 20, 20 });

	EXPECT_EQ(s.getCenter(), other);

	s.fetchPixels();
	EXPECT_EQ(s.getPixelAt(19, 19), Bingo::Colors::TRANSPARENT);
	s.releasePixels();

	EXPECT_EQ(s2->getCenter(), other);

	s2->fetchPixels();
	EXPECT_EQ(s2->getPixelAt(19, 19), Bingo::Colors::TRANSPARENT);
	s2->releasePixels();

	delete s2;
}

TEST(TestSurface, TestConstruct3) {
	auto s = Bingo::Surfaces::Surface(40, 40);
	auto s2 = new Bingo::Surfaces::Surface(40, 40);
	auto other = Bingo::Math::VecN<int, 2>({ 20, 20 });

	EXPECT_EQ(s.getCenter(), other);

	s.fetchPixels();
	EXPECT_EQ(s.getPixelAt(19, 19), Bingo::Colors::TRANSPARENT);
	s.releasePixels();

	EXPECT_EQ(s2->getCenter(), other);

	s2->fetchPixels();
	EXPECT_EQ(s2->getPixelAt(19, 19), Bingo::Colors::TRANSPARENT);
	s2->releasePixels();

	delete s2;
}

TEST(TestSurface, TestConstruct4) {
	auto other = Bingo::Surfaces::Surface("images/CyanSquare.png");
	auto s = Bingo::Surfaces::Surface(other);
	auto s2 = new Bingo::Surfaces::Surface(other);
	auto other2 = Bingo::Math::VecN<int, 2>({ 20, 20 });

	EXPECT_EQ(s.getCenter(), other2);

	s.fetchPixels();
	EXPECT_EQ(s.getPixelAt(19, 19), Bingo::Colors::CYAN);
	s.releasePixels();

	EXPECT_EQ(s2->getCenter(), other2);

	s2->fetchPixels();
	EXPECT_EQ(s2->getPixelAt(19, 19), Bingo::Colors::CYAN);
	s2->releasePixels();

	delete s2;
}

TEST(TestSurface, TestConstruct5) {
	auto other = Bingo::Surfaces::Surface("images/CyanSquare.png");
	auto s = other;
	auto s2 = new Bingo::Surfaces::Surface(40, 40);
	*s2 = other;

	auto other2 = Bingo::Math::VecN<int, 2>({ 20, 20 });

	EXPECT_EQ(s.getCenter(), other2);

	s.fetchPixels();
	EXPECT_EQ(s.getPixelAt(19, 19), Bingo::Colors::CYAN);
	s.releasePixels();

	EXPECT_EQ(s2->getCenter(), other2);

	s2->fetchPixels();
	EXPECT_EQ(s2->getPixelAt(19, 19), Bingo::Colors::CYAN);
	s2->releasePixels();

	delete s2;
}

TEST(TestSurface, TestGetWidthHeight) {
	auto s = Bingo::Surfaces::Surface(40, 60);

	EXPECT_EQ(s.getWidth(), 40);
	EXPECT_EQ(s.getHeight(), 60);
}

TEST(TestSurface, TestSetGetRotation) {
	auto s = Bingo::Surfaces::Surface(40, 60);

	EXPECT_NO_THROW(s.setRotation(273));

	EXPECT_EQ(s.getRotation(), 273);
}

TEST(TestSurface, TestScale) {
	auto s = Bingo::Surfaces::Surface(40, 60);

	EXPECT_NO_THROW(s.setScale(.25f));

	EXPECT_EQ(s.getWidth(), 10);
	EXPECT_EQ(s.getHeight(), 15);
}

TEST(TestSurface, TestScale2) {
	auto s = Bingo::Surfaces::Surface(40, 60);

	EXPECT_NO_THROW(s.setScaleX(.5f));
	EXPECT_NO_THROW(s.setScaleY(1.0f / 6.0f));

	EXPECT_EQ(s.getWidth(), 20);
	EXPECT_EQ(s.getHeight(), 10);
}

TEST(TestSurface, TestSetGetCenter) {
	auto s = Bingo::Surfaces::Surface(40, 60);
	auto other = Bingo::Math::VecN<int, 2>({ 20, 30 });
	auto other2 = Bingo::Math::VecN<int, 2>({ 10, 10 });

	EXPECT_EQ(s.getCenter(), other);

	EXPECT_NO_THROW(s.setCenter({ 10, 10 }));

	EXPECT_EQ(s.getCenter(), other2);
}

TEST(TestSurface, TestSetGetFlip) {
	auto s = Bingo::Surfaces::Surface("images/SplitSquare.png");

	EXPECT_EQ(s.getFlip(), Bingo::Surfaces::Surface::FLIP_NONE);

	std::cout << "Pre flip tests" << std::endl;
	EXPECT_EQ(s.getPixelAt(0, 0), Bingo::Colors::RED);
	EXPECT_EQ(s.getPixelAt(20, 0), Bingo::Colors::YELLOW);
	EXPECT_EQ(s.getPixelAt(0, 20), Bingo::Colors::GREEN);
	EXPECT_EQ(s.getPixelAt(20, 20), Bingo::Colors::CYAN);

	EXPECT_NO_THROW(Bingo::Surfaces::WindowManager::getSingleton().draw(s, 50, 0));

	EXPECT_NO_THROW(s.setFlip(Bingo::Surfaces::Surface::FlipMode::FLIP_HORIZONTAL));

	EXPECT_NO_THROW(Bingo::Surfaces::WindowManager::getSingleton().draw(s, 100, 0));

	EXPECT_EQ(s.getFlip(), Bingo::Surfaces::Surface::FlipMode::FLIP_HORIZONTAL);

	std::cout << "Post flip tests" << std::endl;
	EXPECT_EQ(s.getPixelAt(0, 0), Bingo::Colors::YELLOW);
	EXPECT_EQ(s.getPixelAt(20, 0), Bingo::Colors::RED);
	EXPECT_EQ(s.getPixelAt(0, 20), Bingo::Colors::CYAN);
	EXPECT_EQ(s.getPixelAt(20, 20), Bingo::Colors::GREEN);
}

TEST(TestSurface, TestSetGetFlip2) {
	auto s = Bingo::Surfaces::Surface("images/SplitSquare.png");

	EXPECT_EQ(s.getFlip(), Bingo::Surfaces::Surface::FLIP_NONE);

	std::cout << "Pre flip tests" << std::endl;
	EXPECT_EQ(s.getPixelAt(0, 0), Bingo::Colors::RED);
	EXPECT_EQ(s.getPixelAt(20, 0), Bingo::Colors::YELLOW);
	EXPECT_EQ(s.getPixelAt(0, 20), Bingo::Colors::GREEN);
	EXPECT_EQ(s.getPixelAt(20, 20), Bingo::Colors::CYAN);

	EXPECT_NO_THROW(Bingo::Surfaces::WindowManager::getSingleton().draw(s, 0, 50));

	EXPECT_NO_THROW(s.setFlip(Bingo::Surfaces::Surface::FlipMode::FLIP_VERTICAL));

	EXPECT_NO_THROW(Bingo::Surfaces::WindowManager::getSingleton().draw(s, 00, 100));

	EXPECT_EQ(s.getFlip(), Bingo::Surfaces::Surface::FlipMode::FLIP_VERTICAL);

	std::cout << "Post flip tests" << std::endl;
	EXPECT_EQ(s.getPixelAt(0, 0), Bingo::Colors::GREEN);
	EXPECT_EQ(s.getPixelAt(20, 0), Bingo::Colors::CYAN);
	EXPECT_EQ(s.getPixelAt(0, 20), Bingo::Colors::RED);
	EXPECT_EQ(s.getPixelAt(20, 20), Bingo::Colors::YELLOW);
}

TEST(TestSurface, TestSetGetDrawColor) {
	auto s = Bingo::Surfaces::Surface(40, 60);

	EXPECT_EQ(s.getDrawColor(), Bingo::Colors::WHITE);

	EXPECT_NO_THROW(s.setDrawColor(Bingo::Colors::RED));

	EXPECT_EQ(s.getDrawColor(), Bingo::Colors::RED);
}

TEST(TestSurface, TestSetGetDrawColor_Draw) {
	auto s = Bingo::Surfaces::Surface("images/CyanSquare.png");

	std::cout << "Before draw" << std::endl;
	std::cout << s.getPixelAt(20, 20) << std::endl;
	EXPECT_EQ(s.getPixelAt(20, 20), Bingo::Colors::CYAN);

	// TODO why is drawing this to the screen necessary to read the correct pixel color?
	//Bingo::Surfaces::WindowManager::getSingleton().draw(s, 100, 100);

	s.saveRenderTarget();
	s.setRenderTarget();

	EXPECT_NO_THROW(s.drawRect(0, 0, 40, 40, true));

	s.restoreRenderTarget();

	std::cout << "After draw" << std::endl;
	std::cout << s.getPixelAt(20, 20) << std::endl;
	EXPECT_EQ(s.getPixelAt(20, 20), Bingo::Colors::WHITE);

	Bingo::Surfaces::WindowManager::getSingleton().draw(s, 150, 100);
}

TEST(TestSurface, TestFill) {
	auto s = Bingo::Surfaces::Surface("images/CyanSquare.png");

	EXPECT_EQ(s.getPixelAt(0, 0), Bingo::Colors::CYAN);

	s.saveRenderTarget();
	s.setRenderTarget();

	EXPECT_NO_THROW(s.fill(Bingo::Colors::WHITE));

	s.restoreRenderTarget();

	EXPECT_EQ(s.getPixelAt(0, 0), Bingo::Colors::WHITE);
}

TEST(TestSurface, TestClear) {
	auto s = Bingo::Surfaces::Surface("images/CyanSquare.png");

	EXPECT_EQ(s.getPixelAt(0, 0), Bingo::Colors::CYAN);

	s.saveRenderTarget();
	s.setRenderTarget();

	EXPECT_NO_THROW(s.clear());

	s.restoreRenderTarget();

	EXPECT_EQ(s.getPixelAt(0, 0), Bingo::Colors::TRANSPARENT);
}

TEST(TestSurface, TestRenderTexture) {
	auto s = Bingo::Surfaces::Surface("images/CyanSquare.png");

	EXPECT_NO_THROW(s.renderTexture());
}

// TODO insert tests for draw* functions after TestSetGetDrawColor passes

TEST(TestSurface, TestSetGetColorMod) {
	auto s = Bingo::Surfaces::Surface("images/CyanSquare.png");

	EXPECT_EQ(s.getColorMod(), Bingo::Colors::WHITE);

	EXPECT_NO_THROW(s.setColorMod(Bingo::Colors::RED));

	EXPECT_EQ(s.getColorMod(), Bingo::Colors::RED);
}

TEST(TestSurface, TestSetGetAlphaMod) {
	auto s = Bingo::Surfaces::Surface("images/CyanSquare.png");

	EXPECT_EQ(s.getAlphaMod(), 255);

	EXPECT_NO_THROW(s.setAlphaMod(127));

	EXPECT_EQ(s.getAlphaMod(), 127);
}

TEST(TestSurface, TestViewports) {
	auto s = Bingo::Surfaces::Surface("images/CyanSquare.png");
	uint v;

	EXPECT_NO_THROW(v = s.addViewport(5, 5, 10, 10));

	EXPECT_NO_THROW(s.setViewport(v));

	EXPECT_NO_THROW(s.clearViewport());

	EXPECT_NO_THROW(s.delViewport(v));
}

TEST(TestSurface, TestClips) {
	auto s = Bingo::Surfaces::Surface("images/CyanSquare.png");
	uint v;

	EXPECT_NO_THROW(v = s.addClip(5, 5, 10, 10));

	EXPECT_NO_THROW(s.setClip(v));

	EXPECT_NO_THROW(s.clearClip());

	EXPECT_NO_THROW(s.delClip(v));
}