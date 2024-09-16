#pragma once
#include "pch.h"
#include "VecN.h"

TEST(TestVecN, TestConstruct) {
	auto v = Bingo::Math::VecN<int, 2>();
	auto v2 = new Bingo::Math::VecN<int, 2>();

	EXPECT_NO_THROW(v.getX());
	EXPECT_NO_THROW(v.getY());

	EXPECT_NO_THROW(v2->getX());
	EXPECT_NO_THROW(v2->getY());

	delete v2;
}

TEST(TestVecN, TestConstruct2) {
	auto v = Bingo::Math::VecN<int, 2>({ 1, 2 });
	auto v2 = new Bingo::Math::VecN<int, 2>({ 1, 2 });

	EXPECT_EQ(v.getX(), 1);
	EXPECT_EQ(v.getY(), 2);

	EXPECT_EQ(v2->getX(), 1);
	EXPECT_EQ(v2->getY(), 2);

	delete v2;
}

TEST(TestVecN, TestConstruct3) {
	auto other = Bingo::Math::VecN<double, 2>({ 1.2, 2.8 });
	auto v = Bingo::Math::VecN<int, 2>(other);
	auto v2 = new Bingo::Math::VecN<int, 2>(other);

	EXPECT_EQ(v.getX(), 1);
	EXPECT_EQ(v.getY(), 2);

	EXPECT_EQ(v2->getX(), 1);
	EXPECT_EQ(v2->getY(), 2);

	delete v2;
}

TEST(TestVecN, TestConstruct4) {
	auto other = Bingo::Math::DynVecN<int>(2, { 1, 2 });
	auto v = Bingo::Math::VecN<int, 2>(other);
	auto v2 = new Bingo::Math::VecN<int, 2>(other);

	EXPECT_EQ(v.getX(), 1);
	EXPECT_EQ(v.getY(), 2);

	EXPECT_EQ(v2->getX(), 1);
	EXPECT_EQ(v2->getY(), 2);

	delete v2;
}

TEST(TestVecN, TestFill) {
	auto v = Bingo::Math::VecN<int, 3>();

	EXPECT_NO_THROW(v.fill(42));

	EXPECT_EQ(v.getX(), 42);
	EXPECT_EQ(v.getY(), 42);
	EXPECT_EQ(v.getZ(), 42);
}

TEST(TestVecN, TestBrackets) {
	auto v = Bingo::Math::VecN<int, 3>({ 1, 2, 4 });

	EXPECT_EQ(v[0], 1);
	EXPECT_EQ(v[1], 2);
	EXPECT_EQ(v[2], 4);
}

TEST(TestVecN, TestBrackets2) {
	auto v = Bingo::Math::VecN<int, 3>();

	EXPECT_NO_THROW(v[0] = 1);
	EXPECT_NO_THROW(v[1] = 2);
	EXPECT_NO_THROW(v[2] = 4);

	EXPECT_EQ(v[0], 1);
	EXPECT_EQ(v[1], 2);
	EXPECT_EQ(v[2], 4);
}

TEST(TestVecN, TestSetGet) {
	auto v = Bingo::Math::VecN<int, 3>();

	EXPECT_NO_THROW(v.set(0, 1));
	EXPECT_NO_THROW(v.set(1, 2));
	EXPECT_NO_THROW(v.set(2, 4));

	EXPECT_EQ(v.get(0), 1);
	EXPECT_EQ(v.get(1), 2);
	EXPECT_EQ(v.get(2), 4);
}

TEST(TestVecN, TestSetGet2) {
	auto v = Bingo::Math::VecN<int, 3>();

	EXPECT_NO_THROW(v.set(0, 2));
	EXPECT_NO_THROW(v.set(0, 4));
	EXPECT_NO_THROW(v.set(0, 8));

	EXPECT_NO_THROW(v.get(0) = 1);
	EXPECT_NO_THROW(v.get(1) = 2);
	EXPECT_NO_THROW(v.get(2) = 4);

	EXPECT_EQ(v.get(0), 1);
	EXPECT_EQ(v.get(1), 2);
	EXPECT_EQ(v.get(2), 4);
}

TEST(TestVecN, TestSetGetXYZW) {
	auto v = Bingo::Math::VecN<int, 4>();

	EXPECT_NO_THROW(v.setX(1));
	EXPECT_NO_THROW(v.setY(2));
	EXPECT_NO_THROW(v.setZ(4));
	EXPECT_NO_THROW(v.setW(8));

	EXPECT_EQ(v.getX(), 1);
	EXPECT_EQ(v.getY(), 2);
	EXPECT_EQ(v.getZ(), 4);
	EXPECT_EQ(v.getW(), 8);
}

TEST(TestVecN, TestSetGetXYZW2) {
	auto v = Bingo::Math::VecN<int, 4>();

	EXPECT_NO_THROW(v.setX(2));
	EXPECT_NO_THROW(v.setY(4));
	EXPECT_NO_THROW(v.setZ(8));
	EXPECT_NO_THROW(v.setW(16));

	EXPECT_NO_THROW(v.getX() = 1);
	EXPECT_NO_THROW(v.getY() = 2);
	EXPECT_NO_THROW(v.getZ() = 4);
	EXPECT_NO_THROW(v.getW() = 8);

	EXPECT_EQ(v.getX(), 1);
	EXPECT_EQ(v.getY(), 2);
	EXPECT_EQ(v.getZ(), 4);
	EXPECT_EQ(v.getW(), 8);
}

TEST(TestVecN, TestData) {
	auto v = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 });

	EXPECT_EQ(*(v.data()), 1);
	EXPECT_EQ(*(v.data() + 1), 2);
	EXPECT_EQ(*(v.data() + 2), 4);
	EXPECT_EQ(*(v.data() + 3), 8);
}

TEST(TestVecN, TestBegin) {
	auto v = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 });

	EXPECT_EQ(*(v.begin()), 1);
	EXPECT_EQ(*(v.begin() + 1), 2);
	EXPECT_EQ(*(v.begin() + 2), 4);
	EXPECT_EQ(*(v.begin() + 3), 8);
}

TEST(TestVecN, TestEnd) {
	auto v = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 });

	EXPECT_EQ(v.begin() + 4, v.end());
}

TEST(TestVecN, TestBeginEndIter) {
	auto v = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 });

	for (auto& e : v) {
		std::cout << e << " ";
	}

	std::cout << std::endl;
}

TEST(TestVecN, TestAddTo) {
	auto v = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 });
	auto other = Bingo::Math::VecN<int, 4>({ 4, 2, 4, 8 });

	EXPECT_EQ(v.addTo(0, 3), other);
}

TEST(TestVecN, TestNegate) {
	auto v = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 });
	auto other = Bingo::Math::VecN<int, 4>({ -1, -2, -4, -8 });

	EXPECT_EQ(-v, other);
}

TEST(TestVecN, TestAdd) {
	auto v = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 });
	auto v2 = Bingo::Math::VecN<int, 4>({ 2, 4, 8, 16 });
	auto other = Bingo::Math::VecN<int, 4>({ 3, 6, 12, 24 });

	EXPECT_EQ(v + v2, other);
}

TEST(TestVecN, TestSub) {
	auto v = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 });
	auto v2 = Bingo::Math::VecN<int, 4>({ 2, 4, 8, 16 });
	auto other = Bingo::Math::VecN<int, 4>({ -1, -2, -4, -8 });

	EXPECT_EQ(v - v2, other);
}

TEST(TestVecN, TestMul) {
	auto v = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 });
	auto other = Bingo::Math::VecN<int, 4>({ 2, 4, 8, 16 });

	EXPECT_EQ(v * 2, other);
}

TEST(TestVecN, TestMul2) {
	auto v = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 });
	auto v2 = Bingo::Math::VecN<int, 4>({ 2, 4, 8, 16 });
	auto other = Bingo::Math::VecN<int, 4>({ 2, 8, 32, 128 });

	EXPECT_EQ(v * v2, other);
}

TEST(TestVecN, TestDiv) {
	auto v = Bingo::Math::VecN<double, 4>({ 1.0, 2.0, 4.0, 8.0 });
	auto other = Bingo::Math::VecN<double, 4>({ 0.5, 1.0, 2.0, 4.0 });

	EXPECT_EQ(v / 2, other);
}

TEST(TestVecN, TestDiv2) {
	auto v = Bingo::Math::VecN<int, 4>({ 10, 20, 40, 80 });
	auto v2 = Bingo::Math::VecN<int, 4>({ 2, 4, 8, 16 });
	auto other = Bingo::Math::VecN<int, 4>({ 5, 5, 5, 5 });

	EXPECT_EQ(v / v2, other);
}

TEST(TestVecN, TestAddEq) {
	auto v = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 });
	auto v2 = Bingo::Math::VecN<int, 4>({ 2, 4, 8, 16 });
	auto other = Bingo::Math::VecN<int, 4>({ 3, 6, 12, 24 });

	EXPECT_NO_THROW(v += v2);

	EXPECT_EQ(v, other);
}

TEST(TestVecN, TestSubEq) {
	auto v = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 });
	auto v2 = Bingo::Math::VecN<int, 4>({ 2, 4, 8, 16 });
	auto other = Bingo::Math::VecN<int, 4>({ -1, -2, -4, -8 });

	EXPECT_NO_THROW(v -= v2);

	EXPECT_EQ(v, other);
}

TEST(TestVecN, TestMulEq) {
	auto v = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 });
	auto other = Bingo::Math::VecN<int, 4>({ 2, 4, 8, 16 });

	EXPECT_NO_THROW(v *= 2);

	EXPECT_EQ(v, other);
}

TEST(TestVecN, TestMulEq2) {
	auto v = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 });
	auto v2 = Bingo::Math::VecN<int, 4>({ 2, 4, 8, 16 });
	auto other = Bingo::Math::VecN<int, 4>({ 2, 8, 32, 128 });

	EXPECT_NO_THROW(v *= v2);

	EXPECT_EQ(v, other);
}

TEST(TestVecN, TestDivEq) {
	auto v = Bingo::Math::VecN<double, 4>({ 1.0, 2.0, 4.0, 8.0 });
	auto other = Bingo::Math::VecN<double, 4>({ 0.5, 1.0, 2.0, 4.0 });

	EXPECT_NO_THROW(v /= 2);

	EXPECT_EQ(v, other);
}

TEST(TestVecN, TestDivEq2) {
	auto v = Bingo::Math::VecN<int, 4>({ 10, 20, 40, 80 });
	auto v2 = Bingo::Math::VecN<int, 4>({ 2, 4, 8, 16 });
	auto other = Bingo::Math::VecN<int, 4>({ 5, 5, 5, 5 });

	EXPECT_NO_THROW(v /= v2);

	EXPECT_EQ(v, other);
}

TEST(TestVecN, TestLess) {
	auto v = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 });
	auto other = Bingo::Math::VecN<int, 4>({ 2, 2, 4, 8 });

	EXPECT_TRUE(v < other);
}

TEST(TestVecN, TestLessEq) {
	auto v = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 });
	auto other = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 });
	auto other2 = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 9 });

	EXPECT_TRUE(v <= other);
	EXPECT_TRUE(v <= other2);
}

TEST(TestVecN, TestGreater) {
	auto v = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 9 });
	auto other = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 });

	EXPECT_TRUE(v > other);
}

TEST(TestVecN, TestGreaterEq) {
	auto v = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 });
	auto other = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 });
	auto other2 = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 7 });

	EXPECT_TRUE(v >= other);
	EXPECT_TRUE(v >= other2);
}

TEST(TestVecN, TestEq) {
	auto v = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 });
	auto other = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 });

	EXPECT_TRUE(v == other);
}

TEST(TestVecN, TestNotEq) {
	auto v = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 });
	auto other = Bingo::Math::VecN<int, 4>({ 1, 2, 4, 9 });

	EXPECT_TRUE(v != other);
}

TEST(TestVecN, TestMagnitude) {
	auto v = Bingo::Math::VecN<int, 2>({ 3, 4 });

	EXPECT_EQ(v.magnitude(), 5);
}

TEST(TestVecN, TestMagnitude2) {
	auto v = Bingo::Math::VecN<int, 3>({ 6, 0, 8 });

	EXPECT_EQ(v.magnitude(), 10);
}

TEST(TestVecN, TestNormalize) {
	auto v = Bingo::Math::VecN<double, 3>({ 6, 0, 8 });
	auto other = Bingo::Math::VecN<double, 3>({ 3.0 / 5.0, 0, 4.0 / 5.0 });

	EXPECT_NO_THROW(v.normalize());

	EXPECT_EQ(v, other);
}

TEST(TestVecN, TestNormalizeCopy) {
	auto v = Bingo::Math::VecN<double, 3>({ 6, 0, 8 });
	auto other = Bingo::Math::VecN<double, 3>({ 6.0 / 10.0, 0.0, 8.0 / 10.0 });

	EXPECT_EQ(v.normalizeCopy(), other);
}

TEST(TestVecN, TestDot) {
	auto v = Bingo::Math::VecN<int, 2>({ 1, 2 });
	auto v2 = Bingo::Math::VecN<int, 2>({ 3, 4 });

	EXPECT_EQ(v.dot(v2), 11);
}

TEST(TestVecN, TestDot2) {
	auto v = Bingo::Math::VecN<int, 3>({ 1, 2, 3 });
	auto v2 = Bingo::Math::VecN<int, 3>({ 4, 5, 6 });

	EXPECT_EQ(v.dot(v2), 32);
}

TEST(TestVecN, TestDot3) {
	auto v = Bingo::Math::VecN<int, 3>({ 1, 2, 3 });
	auto v2 = Bingo::Math::VecN<int, 3>({ 1, 5, 7 });

	EXPECT_EQ(v.dot(v2), 32);
}

TEST(TestVecN, TestCross) {
	auto v = Bingo::Math::VecN<int, 2>({ 1, 2 });
	auto v2 = Bingo::Math::VecN<int, 2>({ 3, 4 });

	EXPECT_ANY_THROW(v.cross(v2));
}

TEST(TestVecN, TestCross2) {
	auto v = Bingo::Math::VecN<int, 3>({ 1, 2, 3 });
	auto v2 = Bingo::Math::VecN<int, 3>({ 4, 5, 6 });
	auto other = Bingo::Math::VecN<int, 3>({ -3, 6, -3 });

	EXPECT_EQ(v.cross(v2), other);
}

TEST(TestVecN, TestCross3) {
	auto v = Bingo::Math::VecN<int, 3>({ 1, 2, 3 });
	auto v2 = Bingo::Math::VecN<int, 3>({ 1, 5, 7 });
	auto other = Bingo::Math::VecN<int, 3>({ -1, -4, 3 });

	EXPECT_EQ(v.cross(v2), other);
}

TEST(TestVecN, TestAngleBetween) {
	auto v = Bingo::Math::VecN<int, 2>({ 1, 0 });
	auto v2 = Bingo::Math::VecN<int, 2>({ 0, 1 });

	EXPECT_EQ(v.angleBetween(v2), 90.0);
}

TEST(TestVecN, TestAngleBetween2) {
	auto v = Bingo::Math::VecN<int, 3>({ 1, 0, 1 });
	auto v2 = Bingo::Math::VecN<int, 3>({ 0, 1, 0 });

	EXPECT_EQ(v.angleBetween(v2), 90.0);
}

TEST(TestVecN, TestReverse) {
	auto v = Bingo::Math::VecN<int, 2>({ 1, 2 });
	auto other = Bingo::Math::VecN<int, 2>({ 2, 1 });

	EXPECT_EQ(v.reverse(), other);
}

TEST(TestVecN, TestReverse2) {
	auto v = Bingo::Math::VecN<int, 3>({ 1, 2, 3 });
	auto other = Bingo::Math::VecN<int, 3>({ 3, 2, 1 });

	EXPECT_EQ(v.reverse(), other);
}

TEST(TestVecN, TestPerpendicular) {
	auto v = Bingo::Math::VecN<double, 2>({ -3.0, 10.0 });
	auto other = Bingo::Math::VecN<double, 2>({ 10, 3 });

	EXPECT_EQ(v.perpendicular(), other);
	EXPECT_EQ(v.dot(v.perpendicular()), 0.0);
}

TEST(TestVecN, TestProjectOnto) {
	auto v = Bingo::Math::VecN<double, 2>({ 1, 1 });
	auto v2 = Bingo::Math::VecN<double, 2>({ 2, 2 });
	auto other = Bingo::Math::VecN<double, 2>({ 1, 1 });

	v = v.projectOnto(v2);

	EXPECT_NEAR(v.getX(), other.getX(), 0.0001);
	EXPECT_NEAR(v.getY(), other.getY(), 0.0001);
}

TEST(TestVecN, TestProjectOnto2) {
	auto v = Bingo::Math::VecN<double, 2>({ 3, 4 });
	auto v2 = Bingo::Math::VecN<double, 2>({ 1, 3 });
	auto other = Bingo::Math::VecN<double, 2>({ 1.5, 4.5 });

	v = v.projectOnto(v2);

	EXPECT_NEAR(v.getX(), other.getX(), 0.0001);
	EXPECT_NEAR(v.getY(), other.getY(), 0.0001);
}

TEST(TestDynVecN, TestConstruct) {
	auto v = Bingo::Math::DynVecN<int>();
	auto v2 = new Bingo::Math::DynVecN<int>();

	EXPECT_ANY_THROW(v.getX());
	EXPECT_ANY_THROW(v.getY());

	EXPECT_ANY_THROW(v2->getX());
	EXPECT_ANY_THROW(v2->getY());

	delete v2;
}

TEST(TestDynVecN, TestConstruct2) {
	auto v = Bingo::Math::DynVecN<int>(2);
	auto v2 = new Bingo::Math::DynVecN<int>(2);

	EXPECT_NO_THROW(v.getX());
	EXPECT_NO_THROW(v.getY());

	EXPECT_NO_THROW(v2->getX());
	EXPECT_NO_THROW(v2->getY());

	delete v2;
}

TEST(TestDynVecN, TestConstruct3) {
	auto v = Bingo::Math::DynVecN<int>({ 1, 2 });
	auto v2 = new Bingo::Math::DynVecN<int>({ 1, 2 });

	EXPECT_EQ(v.getX(), 1);
	EXPECT_EQ(v.getY(), 2);

	EXPECT_EQ(v2->getX(), 1);
	EXPECT_EQ(v2->getY(), 2);

	delete v2;
}

TEST(TestDynVecN, TestConstruct4) {
	auto v = Bingo::Math::DynVecN<int>(2, { 1, 2 });
	auto v2 = new Bingo::Math::DynVecN<int>(2, { 1, 2 });

	EXPECT_EQ(v.getX(), 1);
	EXPECT_EQ(v.getY(), 2);

	EXPECT_EQ(v2->getX(), 1);
	EXPECT_EQ(v2->getY(), 2);

	delete v2;
}

TEST(TestDynVecN, TestConstruct5) {
	int other[] = { 1, 2 };
	auto v = Bingo::Math::DynVecN<int>(2, other);
	auto v2 = new Bingo::Math::DynVecN<int>(2, other);

	EXPECT_EQ(v.getX(), 1);
	EXPECT_EQ(v.getY(), 2);

	EXPECT_EQ(v2->getX(), 1);
	EXPECT_EQ(v2->getY(), 2);

	delete v2;
}

TEST(TestDynVecN, TestConstruct6) {
	auto other = Bingo::Math::DynVecN<int>(2, { 1, 2 });
	auto v = Bingo::Math::DynVecN<int>(other);
	auto v2 = new Bingo::Math::DynVecN<int>(other);

	EXPECT_EQ(v.getX(), 1);
	EXPECT_EQ(v.getY(), 2);

	EXPECT_EQ(v2->getX(), 1);
	EXPECT_EQ(v2->getY(), 2);

	delete v2;
}

TEST(TestDynVecN, TestConstruct7) {
	auto other = Bingo::Math::VecN<int, 2>({ 1, 2 });
	auto v = Bingo::Math::DynVecN<int>(other);
	auto v2 = new Bingo::Math::DynVecN<int>(other);

	EXPECT_EQ(v.getX(), 1);
	EXPECT_EQ(v.getY(), 2);

	EXPECT_EQ(v2->getX(), 1);
	EXPECT_EQ(v2->getY(), 2);

	delete v2;
}

TEST(TestDynVecN, TestFill) {
	auto v = Bingo::Math::DynVecN<int>(3);

	EXPECT_NO_THROW(v.fill(42));

	EXPECT_EQ(v.getX(), 42);
	EXPECT_EQ(v.getY(), 42);
	EXPECT_EQ(v.getZ(), 42);
}

TEST(TestDynVecN, TestGetSize) {
	auto v = Bingo::Math::DynVecN<int>(11);

	EXPECT_EQ(v.getSize(), 11);
}

TEST(TestDynVecN, TestSetSize) {
	auto v = Bingo::Math::DynVecN<int>(4);

	v.set(0, 6);
	v.set(1, 7);
	v.set(2, 8);
	v.set(3, 9);

	EXPECT_NO_THROW(v.setSize(2));

	EXPECT_EQ(v.get(0), 6);
	EXPECT_EQ(v.get(1), 7);
}

TEST(TestDynVecN, TestSetSize2) {
	auto v = Bingo::Math::DynVecN<int>(4);

	v.set(0, 6);
	v.set(1, 7);
	v.set(2, 8);
	v.set(3, 9);

	EXPECT_NO_THROW(v.setSize(6));

	EXPECT_EQ(v.get(0), 6);
	EXPECT_EQ(v.get(1), 7);
	EXPECT_EQ(v.get(2), 8);
	EXPECT_EQ(v.get(3), 9);
	EXPECT_EQ(v.get(4), 0);
	EXPECT_EQ(v.get(5), 0);
}

TEST(TestDynVecN, TestBrackets) {
	auto v = Bingo::Math::DynVecN<int>(3, { 1, 2, 4 });

	EXPECT_EQ(v[0], 1);
	EXPECT_EQ(v[1], 2);
	EXPECT_EQ(v[2], 4);
}

TEST(TestDynVecN, TestBrackets2) {
	auto v = Bingo::Math::DynVecN<int>(3);

	EXPECT_NO_THROW(v[0] = 1);
	EXPECT_NO_THROW(v[1] = 2);
	EXPECT_NO_THROW(v[2] = 4);

	EXPECT_EQ(v[0], 1);
	EXPECT_EQ(v[1], 2);
	EXPECT_EQ(v[2], 4);
}

TEST(TestDynVecN, TestSetGet) {
	auto v = Bingo::Math::DynVecN<int>(3);

	EXPECT_NO_THROW(v.set(0, 1));
	EXPECT_NO_THROW(v.set(1, 2));
	EXPECT_NO_THROW(v.set(2, 4));

	EXPECT_EQ(v.get(0), 1);
	EXPECT_EQ(v.get(1), 2);
	EXPECT_EQ(v.get(2), 4);
}

TEST(TestDynVecN, TestSetGet2) {
	auto v = Bingo::Math::DynVecN<int>(3);

	EXPECT_NO_THROW(v.set(0, 2));
	EXPECT_NO_THROW(v.set(0, 4));
	EXPECT_NO_THROW(v.set(0, 8));

	EXPECT_NO_THROW(v.get(0) = 1);
	EXPECT_NO_THROW(v.get(1) = 2);
	EXPECT_NO_THROW(v.get(2) = 4);

	EXPECT_EQ(v.get(0), 1);
	EXPECT_EQ(v.get(1), 2);
	EXPECT_EQ(v.get(2), 4);
}

TEST(TestDynVecN, TestPush_back) {
	auto v = Bingo::Math::DynVecN<int>(4);

	v.setX(1);
	v.setY(2);
	v.setZ(3);
	v.setW(4);

	EXPECT_NO_THROW(v.push_back(5));

	EXPECT_EQ(v.get(4), 5);
	EXPECT_EQ(v.getSize(), 5);
}

TEST(TestDynVecN, TestSetGetXYZW) {
	auto v = Bingo::Math::DynVecN<int>(4);

	EXPECT_NO_THROW(v.setX(1));
	EXPECT_NO_THROW(v.setY(2));
	EXPECT_NO_THROW(v.setZ(4));
	EXPECT_NO_THROW(v.setW(8));

	EXPECT_EQ(v.getX(), 1);
	EXPECT_EQ(v.getY(), 2);
	EXPECT_EQ(v.getZ(), 4);
	EXPECT_EQ(v.getW(), 8);
}

TEST(TestDynVecN, TestSetGetXYZW2) {
	auto v = Bingo::Math::DynVecN<int>(4);

	EXPECT_NO_THROW(v.setX(2));
	EXPECT_NO_THROW(v.setY(4));
	EXPECT_NO_THROW(v.setZ(8));
	EXPECT_NO_THROW(v.setW(16));

	EXPECT_NO_THROW(v.getX() = 1);
	EXPECT_NO_THROW(v.getY() = 2);
	EXPECT_NO_THROW(v.getZ() = 4);
	EXPECT_NO_THROW(v.getW() = 8);

	EXPECT_EQ(v.getX(), 1);
	EXPECT_EQ(v.getY(), 2);
	EXPECT_EQ(v.getZ(), 4);
	EXPECT_EQ(v.getW(), 8);
}

TEST(TestDynVecN, TestData) {
	auto v = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 8 });

	EXPECT_EQ(*(v.data()), 1);
	EXPECT_EQ(*(v.data() + 1), 2);
	EXPECT_EQ(*(v.data() + 2), 4);
	EXPECT_EQ(*(v.data() + 3), 8);
}

TEST(TestDynVecN, TestBegin) {
	auto v = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 8 });

	EXPECT_EQ(*(v.begin()), 1);
	EXPECT_EQ(*(v.begin() + 1), 2);
	EXPECT_EQ(*(v.begin() + 2), 4);
	EXPECT_EQ(*(v.begin() + 3), 8);
}

TEST(TestDynVecN, TestEnd) {
	auto v = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 8 });

	EXPECT_EQ(v.begin() + 4, v.end());
}

TEST(TestDynVecN, TestBeginEndIter) {
	auto v = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 8 });

	for (auto& e : v) {
		std::cout << e << " ";
	}

	std::cout << std::endl;
}

TEST(TestDynVecN, TestAddTo) {
	auto v = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 8 });
	auto other = Bingo::Math::DynVecN<int>(4, { 4, 2, 4, 8 });

	EXPECT_EQ(v.addTo(0, 3), other);
}

TEST(TestDynVecN, TestNegate) {
	auto v = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 8 });
	auto other = Bingo::Math::DynVecN<int>(4, { -1, -2, -4, -8 });

	EXPECT_EQ(-v, other);
}

TEST(TestDynVecN, TestAdd) {
	auto v = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 8 });
	auto v2 = Bingo::Math::DynVecN<int>(4, { 2, 4, 8, 16 });
	auto other = Bingo::Math::DynVecN<int>(4, { 3, 6, 12, 24 });

	EXPECT_EQ(v + v2, other);
}

TEST(TestDynVecN, TestSub) {
	auto v = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 8 });
	auto v2 = Bingo::Math::DynVecN<int>(4, { 2, 4, 8, 16 });
	auto other = Bingo::Math::DynVecN<int>(4, { -1, -2, -4, -8 });

	EXPECT_EQ(v - v2, other);
}

TEST(TestDynVecN, TestMul) {
	auto v = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 8 });
	auto other = Bingo::Math::DynVecN<int>(4, { 2, 4, 8, 16 });

	EXPECT_EQ(v * 2, other);
}

TEST(TestDynVecN, TestMul2) {
	auto v = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 8 });
	auto v2 = Bingo::Math::DynVecN<int>(4, { 2, 4, 8, 16 });
	auto other = Bingo::Math::DynVecN<int>(4, { 2, 8, 32, 128 });

	EXPECT_EQ(v * v2, other);
}

TEST(TestDynVecN, TestDiv) {
	auto v = Bingo::Math::DynVecN<double>(4, { 1.0, 2.0, 4.0, 8.0 });
	auto other = Bingo::Math::DynVecN<double>(4, { 0.5, 1.0, 2.0, 4.0 });

	EXPECT_EQ(v / 2, other);
}

TEST(TestDynVecN, TestDiv2) {
	auto v = Bingo::Math::DynVecN<int>(4, { 10, 20, 40, 80 });
	auto v2 = Bingo::Math::DynVecN<int>(4, { 2, 4, 8, 16 });
	auto other = Bingo::Math::DynVecN<int>(4, { 5, 5, 5, 5 });

	EXPECT_EQ(v / v2, other);
}

TEST(TestDynVecN, TestAddEq) {
	auto v = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 8 });
	auto v2 = Bingo::Math::DynVecN<int>(4, { 2, 4, 8, 16 });
	auto other = Bingo::Math::DynVecN<int>(4, { 3, 6, 12, 24 });

	EXPECT_NO_THROW(v += v2);

	EXPECT_EQ(v, other);
}

TEST(TestDynVecN, TestSubEq) {
	auto v = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 8 });
	auto v2 = Bingo::Math::DynVecN<int>(4, { 2, 4, 8, 16 });
	auto other = Bingo::Math::DynVecN<int>(4, { -1, -2, -4, -8 });

	EXPECT_NO_THROW(v -= v2);

	EXPECT_EQ(v, other);
}

TEST(TestDynVecN, TestMulEq) {
	auto v = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 8 });
	auto other = Bingo::Math::DynVecN<int>(4, { 2, 4, 8, 16 });

	EXPECT_NO_THROW(v *= 2);

	EXPECT_EQ(v, other);
}

TEST(TestDynVecN, TestMulEq2) {
	auto v = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 8 });
	auto v2 = Bingo::Math::DynVecN<int>(4, { 2, 4, 8, 16 });
	auto other = Bingo::Math::DynVecN<int>(4, { 2, 8, 32, 128 });

	EXPECT_NO_THROW(v *= v2);

	EXPECT_EQ(v, other);
}

TEST(TestDynVecN, TestDivEq) {
	auto v = Bingo::Math::DynVecN<double>(4, { 1.0, 2.0, 4.0, 8.0 });
	auto other = Bingo::Math::DynVecN<double>(4, { 0.5, 1.0, 2.0, 4.0 });

	EXPECT_NO_THROW(v /= 2);

	EXPECT_EQ(v, other);
}

TEST(TestDynVecN, TestDivEq2) {
	auto v = Bingo::Math::DynVecN<int>(4, { 10, 20, 40, 80 });
	auto v2 = Bingo::Math::DynVecN<int>(4, { 2, 4, 8, 16 });
	auto other = Bingo::Math::DynVecN<int>(4, { 5, 5, 5, 5 });

	EXPECT_NO_THROW(v /= v2);

	EXPECT_EQ(v, other);
}

TEST(TestDynVecN, TestLess) {
	auto v = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 8 });
	auto other = Bingo::Math::DynVecN<int>(4, { 2, 2, 4, 8 });

	EXPECT_TRUE(v < other);
}

TEST(TestDynVecN, TestLessEq) {
	auto v = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 8 });
	auto other = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 8 });
	auto other2 = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 9 });

	EXPECT_TRUE(v <= other);
	EXPECT_TRUE(v <= other2);
}

TEST(TestDynVecN, TestGreater) {
	auto v = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 9 });
	auto other = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 8 });

	EXPECT_TRUE(v > other);
}

TEST(TestDynVecN, TestGreaterEq) {
	auto v = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 8 });
	auto other = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 8 });
	auto other2 = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 7 });

	EXPECT_TRUE(v >= other);
	EXPECT_TRUE(v >= other2);
}

TEST(TestDynVecN, TestEq) {
	auto v = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 8 });
	auto other = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 8 });

	EXPECT_TRUE(v == other);
}

TEST(TestDynVecN, TestNotEq) {
	auto v = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 8 });
	auto other = Bingo::Math::DynVecN<int>(4, { 1, 2, 4, 9 });

	EXPECT_TRUE(v != other);
}

TEST(TestDynVecN, TestMagnitude) {
	auto v = Bingo::Math::DynVecN<int>(2, { 3, 4 });

	EXPECT_EQ(v.magnitude(), 5);
}

TEST(TestDynVecN, TestMagnitude2) {
	auto v = Bingo::Math::DynVecN<int>(3, { 6, 0, 8 });

	EXPECT_EQ(v.magnitude(), 10);
}

TEST(TestDynVecN, TestNormalize) {
	auto v = Bingo::Math::DynVecN<double>(3, { 6, 0, 8 });
	auto other = Bingo::Math::DynVecN<double>(3, { 3.0 / 5.0, 0, 4.0 / 5.0 });

	EXPECT_NO_THROW(v.normalize());

	EXPECT_EQ(v, other);
}

TEST(TestDynVecN, TestNormalizeCopy) {
	auto v = Bingo::Math::DynVecN<double>(3, { 6, 0, 8 });
	auto other = Bingo::Math::DynVecN<double>(3, { 6.0 / 10.0, 0.0, 8.0 / 10.0 });

	EXPECT_EQ(v.normalizeCopy(), other);
}

TEST(TestDynVecN, TestDot) {
	auto v = Bingo::Math::DynVecN<int>(2, { 1, 2 });
	auto v2 = Bingo::Math::DynVecN<int>(2, { 3, 4 });

	EXPECT_EQ(v.dot(v2), 11);
}

TEST(TestDynVecN, TestDot2) {
	auto v = Bingo::Math::DynVecN<int>(3, { 1, 2, 3 });
	auto v2 = Bingo::Math::DynVecN<int>(3, { 4, 5, 6 });

	EXPECT_EQ(v.dot(v2), 32);
}

TEST(TestDynVecN, TestDot3) {
	auto v = Bingo::Math::DynVecN<int>(3, { 1, 2, 3 });
	auto v2 = Bingo::Math::DynVecN<int>(3, { 1, 5, 7 });

	EXPECT_EQ(v.dot(v2), 32);
}

TEST(TestDynVecN, TestCross) {
	auto v = Bingo::Math::DynVecN<int>(2, { 1, 2 });
	auto v2 = Bingo::Math::DynVecN<int>(2, { 3, 4 });

	EXPECT_ANY_THROW(v.cross(v2));
}

TEST(TestDynVecN, TestCross2) {
	auto v = Bingo::Math::DynVecN<int>(3, { 1, 2, 3 });
	auto v2 = Bingo::Math::DynVecN<int>(3, { 4, 5, 6 });
	auto other = Bingo::Math::DynVecN<int>(3, { -3, 6, -3 });

	EXPECT_EQ(v.cross(v2), other);
}

TEST(TestDynVecN, TestCross3) {
	auto v = Bingo::Math::DynVecN<int>(3, { 1, 2, 3 });
	auto v2 = Bingo::Math::DynVecN<int>(3, { 1, 5, 7 });
	auto other = Bingo::Math::DynVecN<int>(3, { -1, -4, 3 });

	EXPECT_EQ(v.cross(v2), other);
}

TEST(TestDynVecN, TestAngleBetween) {
	auto v = Bingo::Math::DynVecN<int>(2, { 1, 0 });
	auto v2 = Bingo::Math::DynVecN<int>(2, { 0, 1 });

	EXPECT_EQ(v.angleBetween(v2), 90.0);
}

TEST(TestDynVecN, TestAngleBetween2) {
	auto v = Bingo::Math::DynVecN<int>(3, { 1, 0, 1 });
	auto v2 = Bingo::Math::DynVecN<int>(3, { 0, 1, 0 });

	EXPECT_EQ(v.angleBetween(v2), 90.0);
}

TEST(TestDynVecN, TestReverse) {
	auto v = Bingo::Math::DynVecN<int>(2, { 1, 2 });
	auto other = Bingo::Math::DynVecN<int>(2, { 2, 1 });

	EXPECT_EQ(v.reverse(), other);
}

TEST(TestDynVecN, TestReverse2) {
	auto v = Bingo::Math::DynVecN<int>(3, { 1, 2, 3 });
	auto other = Bingo::Math::DynVecN < int>(3, { 3, 2, 1 });

	EXPECT_EQ(v.reverse(), other);
}

TEST(TestDynVecN, TestPerpendicular) {
	auto v = Bingo::Math::DynVecN<double>(2, { -3.0, 10.0 });
	auto other = Bingo::Math::DynVecN<double>(2, { 10, 3 });

	EXPECT_EQ(v.perpendicular(), other);
	EXPECT_EQ(v.dot(v.perpendicular()), 0.0);
}

TEST(TestDynVecN, TestProjectOnto) {
	auto v = Bingo::Math::DynVecN<double>(2, { 1, 1 });
	auto v2 = Bingo::Math::DynVecN<double>(2, { 2, 2 });
	auto other = Bingo::Math::DynVecN<double>(2, { 1, 1 });

	v = v.projectOnto(v2);

	EXPECT_NEAR(v.getX(), other.getX(), 0.0001);
	EXPECT_NEAR(v.getY(), other.getY(), 0.0001);
}

TEST(TestDynVecN, TestProjectOnto2) {
	auto v = Bingo::Math::DynVecN<double>(2, { 3, 4 });
	auto v2 = Bingo::Math::DynVecN<double>(2, { 1, 3 });
	auto other = Bingo::Math::DynVecN<double>(2, { 1.5, 4.5 });

	v = v.projectOnto(v2);

	EXPECT_NEAR(v.getX(), other.getX(), 0.0001);
	EXPECT_NEAR(v.getY(), other.getY(), 0.0001);
}