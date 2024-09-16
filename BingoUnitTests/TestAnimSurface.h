#pragma once
#include "pch.h"
#include "AnimSurface.h"
#include "WindowManager.h"

TEST(TestAnimSurface, TestConstruct) {
	auto as = Bingo::Surfaces::AnimSurface("images/Stoplight.png");
	auto as2 = new Bingo::Surfaces::AnimSurface("images/Stoplight.png");

	EXPECT_EQ(as.getNumFrames(), 0);

	as.fetchPixels();
	EXPECT_EQ(as.getPixelAt(19, 19), Bingo::Colors::RED);
	as.releasePixels();

	EXPECT_EQ(as2->getNumFrames(), 0);

	as2->fetchPixels();
	EXPECT_EQ(as2->getPixelAt(19, 19), Bingo::Colors::RED);
	as2->releasePixels();

	delete as2;
}

TEST(TestAnimSurface, TestConstruct2) {
	auto as = Bingo::Surfaces::AnimSurface("images/Stoplight.png", Bingo::Colors::RED);
	auto as2 = new Bingo::Surfaces::AnimSurface("images/Stoplight.png", Bingo::Colors::RED);

	EXPECT_EQ(as.getNumFrames(), 0);

	as.fetchPixels();
	EXPECT_EQ(as.getPixelAt(19, 19), Bingo::Colors::TRANSPARENT);
	as.releasePixels();

	EXPECT_EQ(as2->getNumFrames(), 0);

	as2->fetchPixels();
	EXPECT_EQ(as2->getPixelAt(19, 19), Bingo::Colors::TRANSPARENT);
	as2->releasePixels();

	delete as2;
}

TEST(TestAnimSurface, TestAddClip) {
	auto as = Bingo::Surfaces::AnimSurface("images/Stoplight.png");

	EXPECT_NO_THROW(as.addClip(0, 0, 40, 40));
	EXPECT_NO_THROW(as.addClip(0, 40, 40, 40));
	EXPECT_NO_THROW(as.addClip(0, 80, 40, 40));
	EXPECT_NO_THROW(as.addClip(0, 120, 40, 40));
}

TEST(TestAnimSurface, TestSetClip) {
	auto as = Bingo::Surfaces::AnimSurface("images/Stoplight.png");

	auto redClip = as.addClip(0, 0, 40, 40);
	auto orgClip = as.addClip(0, 40, 40, 40);
	auto yelClip = as.addClip(0, 80, 40, 40);
	auto grnClip = as.addClip(0, 120, 40, 40);

	EXPECT_NO_THROW(as.setClip(redClip));
	EXPECT_NO_THROW(as.setClip(orgClip));
	EXPECT_NO_THROW(as.setClip(yelClip));
	EXPECT_NO_THROW(as.setClip(grnClip));
	EXPECT_ANY_THROW(as.setClip(-1));
}

TEST(TestAnimSurface, TestGetNumFrames) {
	auto as = Bingo::Surfaces::AnimSurface("images/Stoplight.png");

	as.addClip(0, 0, 40, 40);
	as.addClip(0, 40, 40, 40);
	as.addClip(0, 80, 40, 40);
	as.addClip(0, 120, 40, 40);

	EXPECT_EQ(as.getNumFrames(), 4);
}

TEST(TestAnimSuface, TestGetSetAnimSpeed) {
	auto as = Bingo::Surfaces::AnimSurface("images/Stoplight.png");

	EXPECT_NO_THROW(as.setAnimSpeed(20));

	EXPECT_EQ(as.getAnimSpeed(), 20);
}

TEST(TestAnimSuface, TestAdvanceFrame) {
	auto as = Bingo::Surfaces::AnimSurface("images/Stoplight.png");

	as.addClip(0, 0, 40, 40);
	as.addClip(0, 40, 40, 40);
	as.addClip(0, 80, 40, 40);
	as.addClip(0, 120, 40, 40);

	EXPECT_NO_THROW(as.advanceFrame());
}

TEST(TestAnimSuface, TestReverseFrame) {
	auto as = Bingo::Surfaces::AnimSurface("images/Stoplight.png");

	as.addClip(0, 0, 40, 40);
	as.addClip(0, 40, 40, 40);
	as.addClip(0, 80, 40, 40);
	as.addClip(0, 120, 40, 40);

	EXPECT_NO_THROW(as.reverseFrame());
}