#pragma once
#include "pch.h"
#include "Quaternion.h"

TEST(TestQuaternion, TestContstruct) {
	auto q = Bingo::Math::Quaternion();
	auto q2 = new Bingo::Math::Quaternion();

	EXPECT_EQ(q.getW(), 0.0);
	EXPECT_EQ(q.getX(), 0.0);
	EXPECT_EQ(q.getY(), 0.0);
	EXPECT_EQ(q.getZ(), 0.0);

	EXPECT_EQ(q2->getW(), 0.0);
	EXPECT_EQ(q2->getX(), 0.0);
	EXPECT_EQ(q2->getY(), 0.0);
	EXPECT_EQ(q2->getZ(), 0.0);

	delete q2;
}

TEST(TestQuaternion, TestConstruct2) {
	auto q = Bingo::Math::Quaternion(1.0, 2.0, 4.0, 8.0);
	auto q2 = new Bingo::Math::Quaternion(1.0, 2.0, 4.0, 8.0);

	EXPECT_EQ(q.getW(), 1.0);
	EXPECT_EQ(q.getX(), 2.0);
	EXPECT_EQ(q.getY(), 4.0);
	EXPECT_EQ(q.getZ(), 8.0);

	EXPECT_EQ(q2->getW(), 1.0);
	EXPECT_EQ(q2->getX(), 2.0);
	EXPECT_EQ(q2->getY(), 4.0);
	EXPECT_EQ(q2->getZ(), 8.0);

	delete q2;
}

TEST(TestQuaternion, TestConstruct3) {
	auto p = Bingo::Math::VecN<double, 3>({ 2.0, 4.0, 8.0 });
	auto q = Bingo::Math::Quaternion(p);
	auto q2 = new Bingo::Math::Quaternion(p);

	EXPECT_EQ(q.getX(), 2.0);
	EXPECT_EQ(q.getY(), 4.0);
	EXPECT_EQ(q.getZ(), 8.0);

	EXPECT_EQ(q2->getX(), 2.0);
	EXPECT_EQ(q2->getY(), 4.0);
	EXPECT_EQ(q2->getZ(), 8.0);

	delete q2;
}

TEST(TestQuaternion, TestConstruct4) {
	auto axis = Bingo::Math::VecN<double, 3>({ 1.0, 1.0, 1.0 });
	auto q = Bingo::Math::Quaternion(axis, 90.0);
	auto q2 = new Bingo::Math::Quaternion(axis, 90.0);

	EXPECT_NEAR(q.getW(), 0.707107, 0.000001);
	EXPECT_NEAR(q.getX(), 0.408248, 0.000001);
	EXPECT_NEAR(q.getY(), 0.408248, 0.000001);
	EXPECT_NEAR(q.getZ(), 0.408248, 0.000001);

	EXPECT_NEAR(q2->getW(), 0.707107, 0.000001);
	EXPECT_NEAR(q2->getX(), 0.408248, 0.000001);
	EXPECT_NEAR(q2->getY(), 0.408248, 0.000001);
	EXPECT_NEAR(q2->getZ(), 0.408248, 0.000001);

	delete q2;
}

TEST(TestQuaternion, TestIdentity) {
	auto q = Bingo::Math::Quaternion::identity();

	EXPECT_EQ(q.getW(), 1.0);
	EXPECT_EQ(q.getX(), 0.0);
	EXPECT_EQ(q.getY(), 0.0);
	EXPECT_EQ(q.getZ(), 0.0);
}

TEST(TestQuaternion, TestSetGetW) {
	auto q = Bingo::Math::Quaternion();

	EXPECT_NO_THROW(q.setW(2.5));

	EXPECT_EQ(q.getW(), 2.5);
}

TEST(TestQuaternion, TestSetGetX) {
	auto q = Bingo::Math::Quaternion();

	EXPECT_NO_THROW(q.setX(2.5));

	EXPECT_EQ(q.getX(), 2.5);
}

TEST(TestQuaternion, TestSetGetY) {
	auto q = Bingo::Math::Quaternion();

	EXPECT_NO_THROW(q.setY(2.5));

	EXPECT_EQ(q.getY(), 2.5);
}

TEST(TestQuaternion, TestSetGetZ) {
	auto q = Bingo::Math::Quaternion();

	EXPECT_NO_THROW(q.setZ(2.5));

	EXPECT_EQ(q.getZ(), 2.5);
}

TEST(TestQuaternion, TestMagnitude) {
	auto q = Bingo::Math::Quaternion(0.0, 2.0, 2.0, 2.0);

	EXPECT_NEAR(q.magnitude(), 3.464, 0.001);
}

TEST(TestQuaternion, TestConjugate) {
	auto q = Bingo::Math::Quaternion(0.0, 2.0, 2.0, 2.0);
	auto other = Bingo::Math::Quaternion(0.0, -2.0, -2.0, -2.0);

	EXPECT_EQ(q.conjugate(), other);
}

TEST(TestQuaternion, TestInverse) {
	auto q = Bingo::Math::Quaternion(0.0, 2.0, 2.0, 2.0);
	auto other = Bingo::Math::Quaternion(0.0, -0.167, -0.167, -0.167);

	q = q.inverse();

	EXPECT_NEAR(q.getW(), other.getW(), 0.001);
	EXPECT_NEAR(q.getX(), other.getX(), 0.001);
	EXPECT_NEAR(q.getY(), other.getY(), 0.001);
	EXPECT_NEAR(q.getZ(), other.getZ(), 0.001);
}

TEST(TestQuaternion, TestDot) {
	auto q = Bingo::Math::Quaternion(0.0, 2.0, 2.0, 2.0);
	auto q2 = Bingo::Math::Quaternion(5.0, 4.0, 4.5, 5.0);

	EXPECT_EQ(q.dot(q2), 27.0);
}

TEST(TestQuaternion, TestNegative) {
	auto q = Bingo::Math::Quaternion(1.0, 2.0, 2.5, 3.0);
	auto q2 = Bingo::Math::Quaternion(-1.0, -2.0, -2.5, -3.0);

	EXPECT_EQ(-q, q2);
}

TEST(TestQuaternion, TestSubtract) {
	auto q = Bingo::Math::Quaternion(1.0, 2.0, 2.5, 3.0);
	auto q2 = Bingo::Math::Quaternion(2.0, 4.0, -6.0, 8.0);
	auto other = Bingo::Math::Quaternion(-1.0, -2.0, 8.5, -5.0);

	EXPECT_EQ(q - q2, other);
}

TEST(TestQuaternion, TestMultiply) {
	auto q = Bingo::Math::Quaternion(1.0, 2.0, 2.5, 3.0);
	auto q2 = Bingo::Math::Quaternion(2.0, 4.0, -6.0, 8.0);
	auto other = Bingo::Math::Quaternion(-15.0, 46.0, -5.0, -8.0);

	EXPECT_EQ(q * q2, other);
}

TEST(TestQuaternion, TestMultiply2) {
	auto q = Bingo::Math::Quaternion(1.0, 2.0, 2.5, 3.0);
	auto other = Bingo::Math::Quaternion(3.25, 6.5, 8.125, 9.75);

	EXPECT_EQ(q * 3.25, other);
}

TEST(TestQuaternion, TestDivision) {
	auto q = Bingo::Math::Quaternion(10.0, 20.0, 25.0, 30.0);
	auto other = Bingo::Math::Quaternion(4.0, 8.0, 10.0, 12.0);

	EXPECT_EQ(q / 2.5, other);
}

TEST(TestQuaternion, TestDivision2) {
	auto q = Bingo::Math::Quaternion(1.0, 2.0, 2.5, 3.0);
	auto q2 = Bingo::Math::Quaternion(2.0, 4.0, -6.0, 8.0);
	auto other = Bingo::Math::Quaternion(0.158, -0.317, 0.125, 0.167);

	q = q / q2;

	EXPECT_NEAR(q.getW(), other.getW(), 0.001);
	EXPECT_NEAR(q.getX(), other.getX(), 0.001);
	EXPECT_NEAR(q.getY(), other.getY(), 0.001);
	EXPECT_NEAR(q.getZ(), other.getZ(), 0.001);
}

TEST(TestQuaternion, TestEqual) {
	auto q = Bingo::Math::Quaternion(1.0, 2.0, 2.5, 3.0);
	auto other = Bingo::Math::Quaternion(1.0, 2.0, 2.5, 3.0);

	EXPECT_TRUE(q == other);
}

TEST(TestQuaternion, TestPow) {
	auto q = Bingo::Math::Quaternion(1.0, 0.0, 1.0, 0.0);
	auto other = Bingo::Math::Quaternion(0.0, 0.0, 2.0, 0.0);

	q = pow(q, 2);

	EXPECT_EQ(q, other);
}

TEST(TestQuaternion, TestPow2) {
	auto q = Bingo::Math::Quaternion(0.5, 2.0, 2.5, 3.0);
	auto other = Bingo::Math::Quaternion(-28.75, -37.0, -46.25, -55.5);

	q = Bingo::Math::pow(q, 3);

	EXPECT_EQ(q, other);
}

TEST(TestQuaternion, TestSlerp) {
	auto q = Bingo::Math::Quaternion(1.0, 2.0, 4.0, 8.0);
	auto q2 = Bingo::Math::Quaternion(2.0, 4.0, 8.0, 16.0);
	auto other = Bingo::Math::Quaternion(1.5, 3.0, 6.0, 12.0);

	EXPECT_EQ(Bingo::Math::slerp(q, q2, 0.5), other);
}

TEST(TestQuaternion, TestSlerp2) {
	auto q = Bingo::Math::Quaternion(8.0, 4.0, 2.0, 1.0);
	auto q2 = Bingo::Math::Quaternion(2.0, 4.0, 8.0, 16.0);
	auto other = Bingo::Math::Quaternion(6.5, 4.0, 3.5, 4.75);

	EXPECT_EQ(Bingo::Math::slerp(q, q2, 0.25), other);
}

TEST(TestQuaternion, TestSlerp3) {
	auto q = Bingo::Math::Quaternion(8.0, -4.0, 2.0, 1.0);
	auto q2 = Bingo::Math::Quaternion(2.0, 4.0, -8.0, -16.0);
	auto other = Bingo::Math::Quaternion(6.8, -2.4, 0.0, -2.4);

	q = slerp(q, q2, 0.2);

	EXPECT_NEAR(q.getW(), other.getW(), 0.001);
	EXPECT_NEAR(q.getX(), other.getX(), 0.001);
	EXPECT_NEAR(q.getY(), other.getY(), 0.001);
	EXPECT_NEAR(q.getZ(), other.getZ(), 0.001);
}

TEST(TestQuaternion, TestSquad) {
	auto q = Bingo::Math::Quaternion(8.0, -4.0, 2.0, 1.0);
	auto q2 = Bingo::Math::Quaternion(2.0, 4.0, -8.0, -16.0);
	auto q3 = Bingo::Math::Quaternion(-8.0, 4.0, -2.0, 1.0);
	auto q4 = Bingo::Math::Quaternion(2.0, 4.0, 8.0, 16.0);

	auto other = Bingo::Math::Quaternion(2.387066, -0.019105, 0.182407, 0.254505);

	q = squad(q, q2, q3, q4, 0.2);

	EXPECT_NEAR(q.getW(), other.getW(), 0.000001);
	EXPECT_NEAR(q.getX(), other.getX(), 0.000001);
	EXPECT_NEAR(q.getY(), other.getY(), 0.000001);
	EXPECT_NEAR(q.getZ(), other.getZ(), 0.000001);
}