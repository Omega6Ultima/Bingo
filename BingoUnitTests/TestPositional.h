#pragma once
#include "pch.h"
#include "Positional.h"

TEST(TestPositional, TestConstruct) {
	auto pos = Bingo::Math::Positional();
	auto pos2 = new Bingo::Math::Positional();

	auto other = Bingo::Math::VecN<int, 2>({ 0, 0 });

	EXPECT_EQ(pos.getPos(), other);
	EXPECT_EQ(pos.getPosX(), 0);
	EXPECT_EQ(pos.getPosY(), 0);

	EXPECT_EQ(pos2->getPos(), other);
	EXPECT_EQ(pos2->getPosX(), 0);
	EXPECT_EQ(pos2->getPosY(), 0);

	delete pos2;
}

TEST(TestPositional, TestConstruct2) {
	auto pos = Bingo::Math::Positional(3, 4);
	auto pos2 = new Bingo::Math::Positional(3, 4);

	auto other = Bingo::Math::VecN<int, 2>({ 3, 4 });

	EXPECT_EQ(pos.getPos(), other);
	EXPECT_EQ(pos.getPosX(), 3);
	EXPECT_EQ(pos.getPosY(), 4);

	EXPECT_EQ(pos2->getPos(), other);
	EXPECT_EQ(pos2->getPosX(), 3);
	EXPECT_EQ(pos2->getPosY(), 4);

	delete pos2;
}

TEST(TestPositional, TestConstruct3) {
	auto pos = Bingo::Math::Positional({ 3, 4 });
	auto pos2 = new Bingo::Math::Positional({ 3, 4 });

	auto other = Bingo::Math::VecN<int, 2>({ 3, 4 });

	EXPECT_EQ(pos.getPos(), other);
	EXPECT_EQ(pos.getPosX(), 3);
	EXPECT_EQ(pos.getPosY(), 4);

	EXPECT_EQ(pos2->getPos(), other);
	EXPECT_EQ(pos2->getPosX(), 3);
	EXPECT_EQ(pos2->getPosY(), 4);

	delete pos2;
}

TEST(TestPositional, TestSetGetPos) {
	auto pos = Bingo::Math::Positional({ 3, 4 });
	auto other = Bingo::Math::VecN<int, 2>({ 5, 6 });

	EXPECT_NO_THROW(pos.setPos({ 5, 6 }));

	EXPECT_EQ(pos.getPos(), other);
	EXPECT_EQ(pos.getPosX(), 5);
	EXPECT_EQ(pos.getPosY(), 6);
}

TEST(TestPositional, TestSetGetPos2) {
	auto pos = Bingo::Math::Positional({ 3, 4 });
	auto other = Bingo::Math::VecN<int, 2>({ 5, 6 });

	EXPECT_NO_THROW(pos.setPosX(5));
	EXPECT_NO_THROW(pos.setPosY(6));

	EXPECT_EQ(pos.getPos(), other);
	EXPECT_EQ(pos.getPosX(), 5);
	EXPECT_EQ(pos.getPosY(), 6);
}