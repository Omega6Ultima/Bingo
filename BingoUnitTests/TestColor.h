#pragma once
#include "pch.h"
#include "Color.h"

TEST(TestColor, TestConstruct) {
	auto col = Bingo::Colors::Color(50, 70, 90);
	auto col2 = new Bingo::Colors::Color(50, 70, 90);

	EXPECT_EQ(col.getRed(), 50);
	EXPECT_EQ(col.getGreen(), 70);
	EXPECT_EQ(col.getBlue(), 90);
	EXPECT_EQ(col.getAlpha(), 255);

	EXPECT_EQ(col2->getRed(), 50);
	EXPECT_EQ(col2->getGreen(), 70);
	EXPECT_EQ(col2->getBlue(), 90);
	EXPECT_EQ(col2->getAlpha(), 255);

	delete col2;
}

TEST(TestColor, TestConstruct2) {
	auto col = Bingo::Colors::Color(50, 70, 90, 128);
	auto col2 = new Bingo::Colors::Color(50, 70, 90, 128);

	EXPECT_EQ(col.getRed(), 50);
	EXPECT_EQ(col.getGreen(), 70);
	EXPECT_EQ(col.getBlue(), 90);
	EXPECT_EQ(col.getAlpha(), 128);

	EXPECT_EQ(col2->getRed(), 50);
	EXPECT_EQ(col2->getGreen(), 70);
	EXPECT_EQ(col2->getBlue(), 90);
	EXPECT_EQ(col2->getAlpha(), 128);

	delete col2;
}

TEST(TestColor, TestSetGetRed) {
	auto col = Bingo::Colors::Color(64, 128, 255, 32);

	EXPECT_NO_THROW(col.setRed(16));
	EXPECT_EQ(col.getRed(), 16);
}

TEST(TestColor, TestSetGetGreen) {
	auto col = Bingo::Colors::Color(64, 128, 255, 32);

	EXPECT_NO_THROW(col.setGreen(8));
	EXPECT_EQ(col.getGreen(), 8);
}

TEST(TestColor, TestSetGetBlue) {
	auto col = Bingo::Colors::Color(64, 128, 255, 32);

	EXPECT_NO_THROW(col.setBlue(4));
	EXPECT_EQ(col.getBlue(), 4);
}

TEST(TestColor, TestSetGetAlpha) {
	auto col = Bingo::Colors::Color(64, 128, 255, 32);

	EXPECT_NO_THROW(col.setAlpha(2));
	EXPECT_EQ(col.getAlpha(), 2);
}

TEST(TestColor, TestInverse) {
	EXPECT_EQ(Bingo::Colors::WHITE.inverse(), Bingo::Colors::BLACK);
}

TEST(TestColor, TestInverse2) {
	EXPECT_EQ(Bingo::Colors::CHOCOLATE.inverse().inverse(), Bingo::Colors::CHOCOLATE);
}

TEST(TestColor, TestInverseHue) {
	EXPECT_EQ(Bingo::Colors::ORANGE.inverseHue(), Bingo::Colors::Color(255, 90, 0, 255));
}

TEST(TestColor, TestInverseHue2) {
	EXPECT_EQ(Bingo::Colors::KHAKI.inverseHue().inverseHue(), Bingo::Colors::KHAKI);
}

TEST(TestColor, TestRandom) {
	EXPECT_NO_THROW(Bingo::Colors::Color::random());

	EXPECT_TRUE(Bingo::Colors::Color::random() != Bingo::Colors::Color::random());
}