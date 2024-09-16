#pragma once
#include "pch.h"
#include "CompassDirection.h"

TEST(TestCompassDirection, TestConstruct) {
	auto cd = Bingo::Direction::CompassDirection();
	auto cd2 = new Bingo::Direction::CompassDirection();

	auto noDir = Bingo::Math::VecN<int, 2>({ 0, 0 });

	EXPECT_EQ(cd.getMultipliers(), noDir);
	EXPECT_EQ(cd2->getMultipliers(), noDir);

	delete cd2;
}

TEST(TestCompassDirection, TestConstruct2) {
	auto cd = Bingo::Direction::CompassDirection('n', 'e');
	auto cd2 = Bingo::Direction::CompassDirection('s', 'w');

	EXPECT_EQ(cd.getMultipliers(), cd2.getMultipliers());
}

TEST(TestCompassDirection, TestConstruct3) {
	auto cd = Bingo::Direction::CompassDirection('N', 'E');
	auto cd2 = Bingo::Direction::CompassDirection('S', 'W');

	EXPECT_EQ(cd.getMultipliers(), cd2.getMultipliers());
}

TEST(TestCompassDirection, TestConstruct4) {
	auto cd = Bingo::Direction::CompassDirection(Bingo::Math::VecN<float, 2>({1.0f, 2.0f}));
	auto cd2 = Bingo::Direction::CompassDirection(Bingo::Math::VecN<float, 2>({ -3.0f, -4.0f }));

	EXPECT_EQ(cd.getMultipliers(), cd2.getMultipliers());
}

TEST(TestCompassDirection, TestGetMultipliers) {
	auto cd = Bingo::Direction::CompassDirection('n');

	auto dir = Bingo::Math::VecN<int, 2>({ 0, 1 });

	EXPECT_EQ(cd.getMultipliers(), dir);
}

TEST(TestCompassDirection, TestGetMultipliers2) {
	auto cd = Bingo::Direction::CompassDirection('e');

	auto dir = Bingo::Math::VecN<int, 2>({ 1, 0 });

	EXPECT_EQ(cd.getMultipliers(), dir);
}

TEST(TestCompassDirection, TestAdd) {
	auto cd = Bingo::Direction::CompassDirection('N');
	auto cd2 = Bingo::Direction::CompassDirection('E');
	auto cd3 = Bingo::Direction::CompassDirection('n', 'e');

	EXPECT_EQ(cd + cd2, cd3);
}

TEST(TestCompassDirection, TestSub) {
	auto cd = Bingo::Direction::CompassDirection('n', 'e');
	auto cd2 = Bingo::Direction::CompassDirection('E');
	auto cd3 = Bingo::Direction::CompassDirection('N');

	EXPECT_EQ(cd - cd2, cd3);
}

TEST(TestCompassDirection, TestEquals) {
	auto cd = Bingo::Direction::CompassDirection('n');
	auto cd2 = Bingo::Direction::CompassDirection('n');

	EXPECT_EQ(cd, cd2);
}