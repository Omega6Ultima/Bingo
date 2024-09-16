#pragma once
#include "pch.h"
#include "MinHeap.h"

TEST(TestMinHeap, TestConstruct) {
	auto mh = Bingo::MinHeap<float>();
	auto mh2 = new Bingo::MinHeap<double>();

	EXPECT_TRUE(mh.empty());
	EXPECT_TRUE(mh2->empty());

	delete mh2;
}

enum class TestEnum {
	INVALID = 1,
	ACTIVE = 2,
	INACTIVE = 4,
};

TEST(TestMinHeap, TestConstruct2) {
	auto mh = Bingo::MinHeap<TestEnum>(TestEnum::INVALID);

	mh.push(TestEnum::ACTIVE);

	EXPECT_EQ(mh.pop(), TestEnum::ACTIVE);
}

TEST(TestMinHeap, TestPush) {
	auto mh = Bingo::MinHeap<uint>();

	EXPECT_NO_THROW(mh.push(5));
	EXPECT_NO_THROW(mh.push(3));
	EXPECT_NO_THROW(mh.push(1));
}

TEST(TestMinHeap, TestPop) {
	auto mh = Bingo::MinHeap<int>();

	mh.push(5);
	mh.push(3);
	mh.push(1);

	EXPECT_EQ(mh.pop(), 1);
	EXPECT_EQ(mh.pop(), 3);
	EXPECT_EQ(mh.pop(), 5);
}

TEST(TestMinHeap, TestEmpty) {
	auto mh = Bingo::MinHeap<float>();

	EXPECT_TRUE(mh.empty());

	mh.push(1.0f);

	EXPECT_FALSE(mh.empty());

	mh.pop();

	EXPECT_TRUE(mh.empty());
}

TEST(TestMinHeap, TestExist) {
	auto mh = Bingo::MinHeap<int>();

	mh.push(42);
	mh.push(84);
	mh.push(126);

	EXPECT_TRUE(mh.exist(42));
}

int compFunc(int v1, int v2) {
	return v1 - v2;
}

TEST(TestMinHeap, TestCompFunc) {
	auto mh = Bingo::MinHeap<int>();

	EXPECT_NO_THROW(mh.setComparatorFunc(compFunc));
}

TEST(TestMinHeap, TestSort) {
	auto mh = Bingo::MinHeap<int>();

	mh.push(9);
	mh.push(5);
	mh.push(1);
	mh.push(0);

	EXPECT_EQ(mh.pop(), 0);
	EXPECT_EQ(mh.pop(), 1);
	EXPECT_EQ(mh.pop(), 5);
}

int revCompFunc(int v1, int v2) {
	return v2 - v1;
}

TEST(TestMinHeap, TestSortCustom) {
	auto mh = Bingo::MinHeap<int>();
	mh.setComparatorFunc(revCompFunc);

	mh.push(9);
	mh.push(5);
	mh.push(1);
	mh.push(0);

	EXPECT_EQ(mh.pop(), 9);
	EXPECT_EQ(mh.pop(), 5);
	EXPECT_EQ(mh.pop(), 1);
}
