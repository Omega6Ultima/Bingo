#pragma once

#include "pch.h"
#include "WindowManager.h"
#include "Surface.h"

TEST(TestWindowManager, TestGetWidth) {
	// Width is set in test.cpp
	EXPECT_EQ(Bingo::Surfaces::WindowManager::getSingleton().getWidth(), 600);
}

TEST(TestWindowManager, TestGetHeight) {
	// Height is set in test.cpp
	EXPECT_EQ(Bingo::Surfaces::WindowManager::getSingleton().getHeight(), 400);
}

TEST(TestWindowManager, TestSetGetDrawColor) {
	auto& winMan = Bingo::Surfaces::WindowManager::getSingleton();

	EXPECT_NO_THROW(winMan.setDrawColor(Bingo::Colors::CHOCOLATE));

	EXPECT_EQ(winMan.getDrawColor(), Bingo::Colors::CHOCOLATE);
}

TEST(TestWindowManager, TestGetPixelFormat) {
	EXPECT_NO_THROW(Bingo::Surfaces::WindowManager::getSingleton().getPixelFormat());
}

TEST(TestWindowManager, TestToggleFullscreen) {
	auto& winMan = Bingo::Surfaces::WindowManager::getSingleton();

	EXPECT_NO_THROW(winMan.toggleFullscreen());

	EXPECT_NO_THROW(winMan.toggleFullscreen());
}

TEST(TestWindowManager, TestGetFullscreen) {
	auto& winMan = Bingo::Surfaces::WindowManager::getSingleton();

	EXPECT_NO_THROW(winMan.toggleFullscreen());

	EXPECT_EQ(winMan.getFullscreen(), true);

	EXPECT_NO_THROW(winMan.toggleFullscreen());

	EXPECT_EQ(winMan.getFullscreen(), false);
}

TEST(TestWindowManager, TestAddAutoDraw) {
	shared_ptr<Bingo::Surfaces::Surface> s(new Bingo::Surfaces::Surface("images/CyanSquare.png"));
	s->setPos({ 40, 160 });

	EXPECT_NO_THROW(Bingo::Surfaces::WindowManager::getSingleton().addAutoDraw(s));
}

TEST(TestWindowManager, TestAddAutoDrawAnim) {
	shared_ptr<Bingo::Surfaces::AnimSurface> as(new Bingo::Surfaces::AnimSurface("images/Stoplight.png"));
	as->setPos({ 80, 160 });

	as->addClip(0, 0, 40, 40);
	as->addClip(0, 40, 40, 40);
	as->addClip(0, 80, 40, 40);
	as->addClip(0, 120, 40, 40);
	as->setAnimSpeed(8);

	EXPECT_NO_THROW(Bingo::Surfaces::WindowManager::getSingleton().addAutoDraw(as));
}

TEST(TestWindowManager, TestDisableAutoDraw) {
	auto& winMan = Bingo::Surfaces::WindowManager::getSingleton();
	shared_ptr<Bingo::Surfaces::Surface> s(new Bingo::Surfaces::Surface("images/CyanSquare.png"));
	s->setPos({ 40, 200 });

	EXPECT_NO_THROW(winMan.addAutoDraw(s));

	EXPECT_NO_THROW(winMan.removeAutoDraw(s));
}

TEST(TestWindowManager, TestDisableAutoDrawAnim) {
	auto& winMan = Bingo::Surfaces::WindowManager::getSingleton();
	shared_ptr<Bingo::Surfaces::AnimSurface> as(new Bingo::Surfaces::AnimSurface("images/Stoplight.png"));
	as->setPos({ 80, 160 });

	as->addClip(0, 0, 40, 40);
	as->addClip(0, 40, 40, 40);
	as->addClip(0, 80, 40, 40);
	as->addClip(0, 120, 40, 40);
	as->setAnimSpeed(8);

	EXPECT_NO_THROW(winMan.addAutoDraw(as));

	EXPECT_NO_THROW(winMan.removeAutoDraw(as));
}

// TODO make tests for draw*